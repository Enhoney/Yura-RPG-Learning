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

private:
	static FYuraGameplayTags YuraGameplayTags;
};
