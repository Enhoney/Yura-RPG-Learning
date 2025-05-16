// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "YuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsDelegate, const FGameplayTagContainer& /** EffectAssetTags*/);

DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenSignature);

DECLARE_DELEGATE_OneParam(FForEachAbilitySignature, const FGameplayAbilitySpec&);

// 广播能力状态改变
DECLARE_MULTICAST_DELEGATE_TwoParams(FAbilityStatusChangedSignature, const FGameplayTag& /** AbilityTag*/, const FGameplayTag& /** NewStatusTag*/);

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

	void GrantCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilitiesToGrant);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ForEachAbility(const FForEachAbilitySignature& Delegate);

	void UpdateAbilityStatuses(int32 Level);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	static FGameplayTag GetAbilityInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	static FGameplayTag GetAbilityStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	static FGameplayTag GetAbilityTypeTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	FGameplayAbilitySpec* GetSpecByAbilityTag(const FGameplayTag& AbilityTag);

	// 增加主要属性--客户端调用
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	// 增加主要属性--RPC
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle);

	UFUNCTION(Client, Reliable)
	void ClientAbilityStatusesChanged(const FGameplayTag& AbilityTag, const FGameplayTag& NewStatusTag);

	// 重写的函数，可激活能力变动时广播
	virtual void OnRep_ActivateAbilities() override;

public:
	FEffectAssetTagsDelegate OnEffectAssetTags;

	FAbilitiesGivenSignature OnAbilitiesGivenDelegate;

	// 能力状态改变--包括赋予和解锁
	FAbilityStatusChangedSignature OnAbilityStatusChangedDelegate;

	// 标记初始能力是否赋予--处理时序问题
	bool bStartupAbilitiesGiven = false;
	
};
