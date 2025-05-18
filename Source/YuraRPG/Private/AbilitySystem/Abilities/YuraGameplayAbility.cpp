// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AttributeSets/YuraAttributeSet.h"

FString UYuraGameplayAbility::GetCurrentLevelDescription(int Level)
{
	return FString::Printf(TEXT("<Default>Default Description : %s</>\n<Type>Type : %s</>\n<Level>Level : %d</>"), L"What can I say!!!", L"Offensive", Level);
}

FString UYuraGameplayAbility::GetNextLevelDescription(int Level)
{
	return FString::Printf(TEXT("<Default>Default Description : %s</>\n<Type>Type : %s</>\n<Level>Level : %d</>"), L"Man!!!", L"Offensive", Level);
}

FString UYuraGameplayAbility::GetLockedDescription(int Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: </><Level>%d</>"), Level);
}

void UYuraGameplayAbility::SetWarpingTargetFacingFromLocation(const FVector& TargetLocation)
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		CombatInterface->SetWarpTargetFacing(TargetLocation);
	}
}

FTaggedMontage UYuraGameplayAbility::GetRandTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 RandIndex = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[RandIndex];
	}

	return FTaggedMontage();
}

float UYuraGameplayAbility::GetManaCost(float InLevel /*= 1.0f*/) const
{
	float OutManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		// 遍历GE的Modifiers
		for (const FGameplayModifierInfo& Mod : CostEffect->Modifiers)
		{
			// 如果修改的是Mana
			if (Mod.Attribute == UYuraAttributeSet::GetManaAttribute())
			{
				// 拿到对应等级的ModifierMagnitude，只有Mod在使用ScalableFloat或者硬编码的时候才能拿到正确的值
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, OutManaCost);
			}
		}
	}

	// 因为这里拿到的是负数，所以要返回相反数
	return -OutManaCost;
}

float UYuraGameplayAbility::GetCooldown(float InLevel /*= 1.0f*/) const
{
	float OutCooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		if (CooldownEffect->DurationPolicy == EGameplayEffectDurationType::HasDuration)
		{
			CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, OutCooldown);
		}
	}

	return OutCooldown;
}
