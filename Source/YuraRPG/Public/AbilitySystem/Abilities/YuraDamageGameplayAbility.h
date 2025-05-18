// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/YuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "YuraDamageGameplayAbility.generated.h"

struct FGameplayTag;

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraDamageGameplayAbility : public UYuraGameplayAbility
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void GetDamageSpecHandle(FGameplayEffectSpecHandle& OutDamageEffectSpecHandle);

	// 从表格中，根据伤害类型获取当前等级的伤害
	float GetBaseDamageTyped(const FGameplayTag& DamageType, int32 Level) const;

protected:

	// 技能伤害类型和对应的基础伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	// 伤害的GE
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
};
