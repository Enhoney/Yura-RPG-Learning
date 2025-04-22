// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "YuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsDelegate, const FGameplayTagContainer& /** EffectAssetTags*/);

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	void GrantCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant);


	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	void AbilityInputTagReleased(const FGameplayTag& InputTag);


protected:
	void EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle);

public:
	FEffectAssetTagsDelegate OnEffectAssetTags;
	
};
