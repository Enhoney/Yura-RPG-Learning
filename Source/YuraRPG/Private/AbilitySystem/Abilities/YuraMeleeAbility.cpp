// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraMeleeAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UYuraMeleeAbility::CauseMeleeDamageToTarget(AActor* TargetActor)
{
	// 生成Handle
	FGameplayEffectSpecHandle DamageSpecHandle;
	GetDamageSpecHandle(DamageSpecHandle);

	// 获取目标身上的ASC
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC)
	{
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
	}
}
