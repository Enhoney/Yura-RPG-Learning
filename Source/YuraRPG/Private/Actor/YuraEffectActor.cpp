// Copyright Muci Yun.


#include "Actor/YuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"


// Sets default values
AYuraEffectActor::AYuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
}

void AYuraEffectActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RunningTime += DeltaSeconds;
	// 周期
	if (RunningTime >  2 * PI / SinPeriod)
	{
		RunningTime = 0.f;
	}

	if (bRotates)
	{
		const FRotator DeltaRotation(0.f, DeltaSeconds * RotateRate, 0.f);
		AddActorWorldRotation(DeltaRotation);
	}
	if (bSinMovement)
	{
		const float SinMovementDistance = SinAmplitude * FMath::Sin(RunningTime * SinPeriod);
		SetActorLocation(ActorInitialLocation + FVector(0.f, 0.f, SinMovementDistance));
	}
}


void AYuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	ActorInitialLocation = GetActorLocation();

}

bool AYuraEffectActor::ApplyGameplayEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (!bApplyToEnemy && TargetActor->ActorHasTag(FName("YuraCharacter.Enemy")))
	{
		return false;
	}

	// 查找ASC，我们有两种方法，一种是直接调用接口的GetAbilitySystemComponent函数
	// 还有一种更通用的，由蓝图静态函数库提供的方法，这种更好更通用，如果我们没有实现这个接口，但是也为它添加了ASC，
	// 在这种情况下，依然可以查找到（通过FindComponentByClass）
	// 当然，如果你习惯于为每一个添加ASC的类都实现这个接口，调用接口的方法也是没问题的
	UAbilitySystemComponent* TargrtASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargrtASC == nullptr)
	{
		return false;
	}

	check(GameplayEffectClass);
	FGameplayEffectContextHandle GameplayEffectContextHandle = TargrtASC->MakeEffectContext();
	// 效果的直接来源
	GameplayEffectContextHandle.AddSourceObject(this);
	// 生成Spec
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = TargrtASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, GameplayEffectContextHandle);
	
	// 施加效果
	const FActiveGameplayEffectHandle ActiveGEHandle = TargrtASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());

	// 获取DurationType
	const EGameplayEffectDurationType GEDurationType = (GameplayEffectSpecHandle.Data.Get())->Def.Get()->DurationPolicy;
	// 如果是无限的，并且后面会移除的，就保存句柄
	if (GEDurationType == EGameplayEffectDurationType::Infinite && InfiniteEffectRemovePolicy ==EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveGEHandle, TargrtASC);
	}

	// 如果是即时效果或者持续效果，并且设置为一次性的，施加完就销毁这个Actor
	if (bDestroyOnEffectApplicated && GEDurationType != EGameplayEffectDurationType::Infinite)
	{
		Destroy();
	}

	return true;

}

void AYuraEffectActor::OnBeginOverlap(AActor* TargetActor)
{
	// 如果不能施加给敌人，并且重叠的是敌人，就啥也不敢
	if (!bApplyToEnemy && TargetActor->ActorHasTag(FName("YuraCharacter.Enemy")))
	{
		return;
	}

	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyGameplayEffectToActor(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyGameplayEffectToActor(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyGameplayEffectToActor(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AYuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (!bApplyToEnemy && TargetActor->ActorHasTag(FName("YuraCharacter.Enemy")))
	{
		return;
	}

	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyGameplayEffectToActor(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyGameplayEffectToActor(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyGameplayEffectToActor(TargetActor, InfiniteGameplayEffectClass);
	}

	// Infinite GE的移除
	
	if (InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargrtASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargrtASC))
		{
			return;
		}
		TArray<FActiveGameplayEffectHandle> EffectHandleToRemove;
		// 容器遍历的时候不能直接移除
		for (auto TmpHandlePair : ActiveEffectHandles)
		{
			if (TmpHandlePair.Value == TargrtASC)
			{
				// 效果移除，存储一下要移除的handle
				TargrtASC->RemoveActiveGameplayEffect(TmpHandlePair.Key, 1);
				EffectHandleToRemove.Add(TmpHandlePair.Key);
			}
		}
		// 从容器中移除
		// 如果这个Key不存在，就触发断言
		for (auto& TmpHandle : EffectHandleToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(TmpHandle);
		}
	}
}

void AYuraEffectActor::StartSinMovement()
{
	bSinMovement = true;
	ActorInitialLocation = GetActorLocation();
}

void AYuraEffectActor::StartRotateMovement()
{
	bRotates = true;
}


