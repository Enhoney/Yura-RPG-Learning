// Copyright Muci Yun.


#include "AbilitySystem/YuraAbilitySystemGlobals.h"

#include "YuraAbilityTypes.h"

FGameplayEffectContext* UYuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FYuraGameplayEffectContext();
}
