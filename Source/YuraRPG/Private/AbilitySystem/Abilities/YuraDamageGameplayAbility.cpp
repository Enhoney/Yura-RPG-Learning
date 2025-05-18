// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraDamageGameplayAbility.h"

#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


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

float UYuraDamageGameplayAbility::GetBaseDamageTyped(const FGameplayTag& DamageType, int32 Level) const
{

	checkf(DamageTypes.Contains(DamageType), TEXT("DamageTypes don't have the TypeTag: %s"), *DamageType.ToString());
	return DamageTypes[DamageType].GetValueAtLevel(Level);
}


