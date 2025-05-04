// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/YuraGameplayAbility.h"
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
	// 设置WarpingTargetFacing
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetWarpingTargetFacingFromLocation(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void GetDamageSpecHandle(FGameplayEffectSpecHandle& OutDamageEffectSpecHandle);

protected:

	// 技能伤害类型和对应的基础伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	// 伤害的GE
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
};
