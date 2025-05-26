// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/YuraGameplayAbility.h"
#include "YuraPassiveGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraPassiveGameplayAbility : public UYuraGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

protected:
	void RecvAbilitDeactive(const FGameplayTag& InAbilityTag);
	
};
