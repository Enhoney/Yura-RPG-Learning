// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "YuraAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	/** Should allocate a project specific GameplayEffectContext struct. Caller is responsible for deallocation */
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
