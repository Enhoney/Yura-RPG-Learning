// Copyright Muci Yun.


#include "YuraGameplayTags.h"
#include "GameplayTagsManager.h"

FYuraGameplayTags FYuraGameplayTags::YuraGameplayTags;

void FYuraGameplayTags::InitializeGameplayTags()
{
	// Vital Attributes Tags
	YuraGameplayTags.Attribute_Vital_Health =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Vital.Health"), FString("Chartacter's Health."));
	YuraGameplayTags.Attribute_Vital_Mana =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Vital.Mana"), FString("Chartacter's Mana."));

	// Primary Attributes Tags
	YuraGameplayTags.Attribute_Primary_Strength =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Strength"), FString("Increase physical damage."));
	YuraGameplayTags.Attribute_Primary_Intelligence =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Intelligence"), FString("Increase MaxMana and Mana recovery speed and magic damage."));
	YuraGameplayTags.Attribute_Primary_Resilience =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Resilience"), FString("Increase Armor and armor penetration."));
	YuraGameplayTags.Attribute_Primary_Vigor =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Vigor"), FString("Increase MaxHealth and Health recovery speed."));

	// Secondary Attributes Tags
	YuraGameplayTags.Attribute_Secondary_MaxHealth =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxHealth"), FString("Chartacter's MaxHealth."));
	YuraGameplayTags.Attribute_Secondary_MaxMana =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxMana"), FString("Chartacter's MaxMana."));
	YuraGameplayTags.Attribute_Secondary_Armor = 
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Armor"), FString("Resuce damage taken, improves Block Chance."));
	YuraGameplayTags.Attribute_Secondary_ArmorPenetration =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ArmorPenetration"), FString("Improves CriticalHit Chance and CriticalHit damage."));
	YuraGameplayTags.Attribute_Secondary_BlockChance =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.BlockChance"), FString("The probability reduces the damage received by half."));
	YuraGameplayTags.Attribute_Secondary_CriticalHitChance =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitChance"), FString("Probability causes critical hit damage."));
	YuraGameplayTags.Attribute_Secondary_CriticalHitDamage =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitDamage"), FString("The multiplier of critical hit damage."));
	YuraGameplayTags.Attribute_Secondary_CriticalHitResistance =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitResistance"), FString("Reduce the local critical hit rate."));
	YuraGameplayTags.Attribute_Secondary_HealthRegeneration =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.HealthRegeneration"), FString("Health points automatically restored per second."));
	YuraGameplayTags.Attribute_Secondary_ManaRegeneration =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ManaRegeneration"), FString("Mana points automatically restored per second."));

	// Ability Input Tags
	YuraGameplayTags.InputTag_LMB =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("InputTag Left Mouse Button."));
	YuraGameplayTags.InputTag_RMB =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("InputTag Right Mouse Button."));
	YuraGameplayTags.InputTag_1 =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("InputTag 1 Key."));
	YuraGameplayTags.InputTag_2 =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("InputTag 2 Key."));
	YuraGameplayTags.InputTag_3 =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("InputTag 3 Key."));
	YuraGameplayTags.InputTag_4 =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("InputTag 4 Key."));

}
