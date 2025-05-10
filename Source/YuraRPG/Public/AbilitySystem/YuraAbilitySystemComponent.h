// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "YuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsDelegate, const FGameplayTagContainer& /** EffectAssetTags*/);

DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenSignature);

DECLARE_DELEGATE_OneParam(FForEachAbilitySignature, const FGameplayAbilitySpec&);

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

	void ForEachAbility(const FForEachAbilitySignature& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	static FGameplayTag GetAbilityInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle);

	// 
	virtual void OnRep_ActivateAbilities() override;

public:
	FEffectAssetTagsDelegate OnEffectAssetTags;

	FAbilitiesGivenSignature OnAbilitiesGivenDelegate;

	// 标记初始能力是否赋予--处理时序问题
	bool bStartupAbilitiesGiven = false;
	
};
