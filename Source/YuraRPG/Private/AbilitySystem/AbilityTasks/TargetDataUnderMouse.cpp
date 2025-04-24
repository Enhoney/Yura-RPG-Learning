// Copyright Muci Yun.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"


UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	// 是否为主机代理
	const bool bLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bLocallyControlled)
	{
		// 发送数据到服务器
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey PredictionKey = GetActivationPredictionKey();
		// 在服务器上，监听客户端发来的数据
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey)
			.AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		// 检查这个时候有没有接收到数据，在这个AbilityTargetDataSetDelegate代理广播的时候，就会返回true
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);

		if (!bCalledDelegate)
		{
			// 如果没有接收到数据，就进入监听模式，等待客户端数据过来
			SetWaitingOnRemotePlayerData();
		}
	}

	
}


void UTargetDataUnderMouse::SendMouseCursorData()
{
	// 使用ScopedPredictionKey需要这样一个Window
	// 就像临界区锁那样，意味着这个代码块中的行为都可以被预测
	// 它会刷新AbilitySystemComponent->ScopedPredictionKey
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());

	// 拿到鼠标下方的位置，这个需要Controller
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	// 这就是GameplayTargetData的创建方式，还是要用new嘛
	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
	TargetData->HitResult = CursorHit;

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(TargetData);
	// PredictionKey是预测需要的东西，这里我们就直接传递这个Ability激活的时候的Key就行了
	// 这个tag没用上，但是得传递参数
	// 这么大一串东西，实际上目的就是将这个HitResult发送给服务器
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), TargetDataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

	// 确保这个时候可以进行广播
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
	}

}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	// 告诉ASC，已经接收到数据了，不必担心，也不用保存这个数据
	AbilitySystemComponent.Get()->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	// 就执行我们自己的逻辑，将数据广播出去
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
