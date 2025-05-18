// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "YuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsDelegate, const FGameplayTagContainer& /** EffectAssetTags*/);

DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenSignature);

DECLARE_DELEGATE_OneParam(FForEachAbilitySignature, const FGameplayAbilitySpec&);

// 广播能力状态改变
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChangedSignature, const FGameplayTag& /** AbilityTag*/, const FGameplayTag& /** NewStatusTag*/, int32 /** NewLevel*/);

// 广播技能装备与卸载
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipAndUnloadSignature, const FGameplayTag& /** AbilityTag*/, 
	const FGameplayTag& /** NewStatusTag*/, const FGameplayTag& /** InputSlot*/, const FGameplayTag& /** PreInputSlot*/);

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


	// 根据Abilitytag拿到描述信息--只会在客户端调用
	bool GetAbilityCurrentDescription(const FGameplayTag& InAbilityTag, const class UAbilityInfo* InAbilityInfo, FString& OutCurDescription, FString& OutNextLevelDescription);


	// 根据Ability Tag查找StatusTag
	// 可以在外部使用
	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemComponent")
	FGameplayTag GetStatusByAbilityTag(const FGameplayTag& InAbilityTag);
	// 根据AbilityTag获取InputTag
	FGameplayTag GetInputByAbilityTag(const FGameplayTag& InAbilityTag);

	FGameplayAbilitySpec* GetSpecByAbilityTag(const FGameplayTag& AbilityTag);

	FGameplayAbilitySpec* GetSpecByAbilityInputTag(const FGameplayTag& AbilityInputTag);

	// 增加主要属性--客户端调用
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	// 增加主要属性--RPC
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	// 消耗技能点
	UFUNCTION(Server, Reliable)
	void ServerSpendignSpellPoint(const FGameplayTag& AbilityTag, int32 SpellPointsToSpend);

	// 装备技能
	UFUNCTION(Server, Reliable)
	void ServerEquipSpellToInputSlot(const FGameplayTag& AbilityTag, const FGameplayTag& TargetInputTag);

protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle);

	UFUNCTION(Client, Reliable)
	void ClientAbilityStatusesChanged(const FGameplayTag& AbilityTag, const FGameplayTag& NewStatusTag, int32 NewAbilityLevel);

	// 装备技能改变，客户端回调，最终目的是通知SpellMenu和Overlay更新
	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& NewStatusTag, const FGameplayTag& InputSlot, const FGameplayTag& PreInputSlot);

	// 重写的函数，可激活能力变动时广播
	virtual void OnRep_ActivateAbilities() override;

	// 服务器上调配用--卸载技能
	void UnloadAbilityEquipped(FGameplayAbilitySpec* AbilitySpec);

	// 服务器上调配用--装备技能
	bool EquipAbility(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& TargetInputTag);

public:
	FEffectAssetTagsDelegate OnEffectAssetTags;

	FAbilitiesGivenSignature OnAbilitiesGivenDelegate;

	// 能力状态改变--包括赋予和解锁
	FAbilityStatusChangedSignature OnAbilityStatusChangedDelegate;

	// 技能装备与卸载
	FAbilityEquipAndUnloadSignature OnAbilityEquipAndUnloadDelegate;

	// 标记初始能力是否赋予--处理时序问题
	bool bStartupAbilitiesGiven = false;
	
};
