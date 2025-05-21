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
	const float ScalableDamage = SpellBaseDamage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageTypeTag, ScalableDamage);


	OutDamageEffectSpecHandle = DamageSpecHandle;
}

float UYuraDamageGameplayAbility::GetBaseDamageTyped(const FGameplayTag& DamageType, int32 Level) const
{

	if (DamageTypeTag.MatchesTagExact(DamageType))
	{
		return SpellBaseDamage.GetValueAtLevel(Level);
	}
	return 0.f;
	
}

FDamageEffectParams UYuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor)  const
{
	FDamageEffectParams OutParams;
	OutParams.WorldContextObject = GetAvatarActorFromActorInfo();
	OutParams.DamageGameplayEffectClass = DamageEffectClass;
	OutParams.SourceASC = GetAbilitySystemComponentFromActorInfo();
	OutParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	OutParams.AbilityBaseDamage = SpellBaseDamage.GetValueAtLevel(GetAbilityLevel());
	OutParams.AbilityLevel = GetAbilityLevel();
	OutParams.DamageType = DamageTypeTag;
	OutParams.DeathImpulseMagnitude = DeathImpulseMagnitude;
	OutParams.DebuffChance = DebuffChance;
	OutParams.DebuffDuration = DebuffDuration;
	OutParams.DebuffFrequency = DebuffFrequency;
	OutParams.DebuffBaseDamage = DebuffBaseDamage;

	// 击退
	OutParams.KnockbackChance = KnockbackChance;
	OutParams.KnockbackForceMagnitude = KnockbackForceMagnitude;

	if (IsValid(TargetActor))
	{
		// 击退判定
		if (FMath::RandRange(1, 100) < OutParams.KnockbackChance * 100)
		{
			FRotator KnockbackRotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
			KnockbackRotation.Pitch = 45.f;

			OutParams.KnockbackVector = KnockbackRotation.Vector().GetSafeNormal() * OutParams.KnockbackForceMagnitude;
			// 击杀冲量
			OutParams.DeathImpulse = KnockbackRotation.Vector().GetSafeNormal() * OutParams.DeathImpulseMagnitude;
		}
	}

	return OutParams;
}

