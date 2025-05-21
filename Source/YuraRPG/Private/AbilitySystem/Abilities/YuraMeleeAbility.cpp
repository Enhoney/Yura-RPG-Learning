// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraMeleeAbility.h"
#include "AbilitySystemComponent.h"
#include "YuraAbilitySystemLibrary.h"

void UYuraMeleeAbility::CauseMeleeDamageToTarget(AActor* TargetActor)
{
	// 生成参数
	FDamageEffectParams DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(TargetActor);

	UYuraAbilitySystemLibrary::ApplyDamageEffectByParams(DamageEffectParams);
}
