// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraDamageGameplayAbility.h"

#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UYuraDamageGameplayAbility::SetWarpingTargetFacingFromLocation(const FVector& TargetLocation)
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		CombatInterface->SetWarpTargetFacing(TargetLocation);
	}
}

void UYuraDamageGameplayAbility::GetDamageSpecHandle(FGameplayEffectSpecHandle& OutDamageEffectSpecHandle)
{
	// 我们要在这里给他一个GameplayEffectSpec
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle DamageEffectContextHandle = ASC->MakeEffectContext();
	// 添加来源
	DamageEffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	FGameplayEffectSpecHandle DamageSpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), DamageEffectContextHandle);

	// 设置基础伤害
	// 从表格中拿到数值
	for (const TPair<FGameplayTag, FScalableFloat>& Pair : DamageTypes)
	{
		const float ScalableDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScalableDamage);
	}

	OutDamageEffectSpecHandle = DamageSpecHandle;
}

FTaggedMontage UYuraDamageGameplayAbility::GetRandTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 RandIndex = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[RandIndex];
	}

	return FTaggedMontage();
}

