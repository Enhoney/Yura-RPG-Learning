// Copyright Muci Yun.


#include "AsyncTasks/WaitForCooldownChange.h"
#include "AbilitySystemComponent.h"


UWaitForCooldownChange* UWaitForCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* InASC, const FGameplayTag& InCooldownTag)
{
	UWaitForCooldownChange* WatiForCooldownChange = NewObject<UWaitForCooldownChange>();

	WatiForCooldownChange->ASC = InASC;
	WatiForCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid(InASC) || !InCooldownTag.IsValid())
	{
		WatiForCooldownChange->EndTask();
		return nullptr;
	}

	// 通过这个知道什么时候冷却结束（虽然这个也可以在冷却开始的时候被触发，但是无法拿到具体的冷却时间）
	// 注册监听Tag添加和移除事件回调
	InASC->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WatiForCooldownChange, &UWaitForCooldownChange::CooldownChanged);

	// 通过这个知道冷却什么时候开始，冷却时间的值是多少
	// 注册监听GE施加给自己的回调，这个代理会在服务器和客户端都进行调用,，并且只针对HasDuration的GE
	InASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WatiForCooldownChange, &UWaitForCooldownChange::OnActiveAdded);

	
	return WatiForCooldownChange;
}

void UWaitForCooldownChange::EndTask()
{
	if (!IsValid(ASC)) return;
	// 解除绑定的代理
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	// 准备进行销毁
	SetReadyToDestroy();
	// 标记为可回收，这相当于手动加快了它的回收过程
	MarkAsGarbage();
}

void UWaitForCooldownChange::CooldownChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	// 如果是冷却结束
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}

}

void UWaitForCooldownChange::OnActiveAdded(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// 先看看上面有没有CooldownTag
	FGameplayTagContainer EffectAssetTags;
	SpecApplied.GetAllAssetTags(EffectAssetTags);
	FGameplayTagContainer EffectGrantTags;
	SpecApplied.GetAllGrantedTags(EffectGrantTags);
	// 这个Tag不一定在哪呢
	if (EffectAssetTags.HasTagExact(CooldownTag) || EffectGrantTags.HasTagExact(CooldownTag))
	{
		// 找到冷却时间，这里我们通过GE来查找
		FGameplayEffectQuery EffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		// 这个查找方式有点麻烦啊，但是很通用，完全绕开了GA
		// 因为可能存在多个GE都使用了这个Tag，所以是一个数组
		TArray<float> TimesRemaining = InASC->GetActiveEffectsTimeRemaining(EffectQuery);
		// 那么，我们要做的就是找到最长的那个
		float RemainingTime = 0.f;
		if (TimesRemaining.Num() > 0)
		{
			for (int32 i = 0; i < TimesRemaining.Num(); i++)
			{
				if (RemainingTime < TimesRemaining[i])
				{
					RemainingTime = TimesRemaining[i];
				}
			}

			CooldownStart.Broadcast(RemainingTime);
		}
	}
}
