// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * FYuraGameplayTags
 * 
 * Singleton containing native gameplay tags
 */
struct YURARPG_API FYuraGameplayTags
{
public:
	static const FYuraGameplayTags& Get() { return YuraGameplayTags; }

	static void InitializeGameplayTags();


public:
	/** 
	 * Attributes
	 */

	// Vital Attributes Tags
	FGameplayTag Attribute_Vital_Health;
	FGameplayTag Attribute_Vital_Mana;

	// Primary Attributes Tags
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;

	// Secondary Attributes Tags
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;

	// Ability Input Tags
	// 这些tag和实际的按键不一定是完全对应的
	FGameplayTag InputTag_LMB;	// 鼠标左键
	FGameplayTag InputTag_RMB;	// 鼠标右键
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

private:
	static FYuraGameplayTags YuraGameplayTags;
};
