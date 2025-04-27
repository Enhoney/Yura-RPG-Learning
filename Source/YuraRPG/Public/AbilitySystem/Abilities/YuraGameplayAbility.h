// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "YuraGameplayAbility.generated.h"

struct FGameplayTag;

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag StartUpInputTag;

	// 技能基础伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FScalableFloat BaseDamage;
	
};
