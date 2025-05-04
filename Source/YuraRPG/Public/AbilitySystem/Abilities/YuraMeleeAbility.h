// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/YuraDamageGameplayAbility.h"
#include "YuraMeleeAbility.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraMeleeAbility : public UYuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Melee Attack Ability")
	void CauseMeleeDamageToTarget(AActor* TargetActor);

	
};
