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

	// 伤害抗性
	YuraGameplayTags.Attribute_Resilience_Fire =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resilience.Fire"), FString("Flame resistance."));
	YuraGameplayTags.Attribute_Resilience_Physic =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resilience.Physic"), FString("Physic resistance."));
	YuraGameplayTags.Attribute_Resilience_Lighting =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resilience.Lighting"), FString("Lighting resistance."));
	YuraGameplayTags.Attribute_Resilience_Arcane =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resilience.Arcane"), FString("Arcane resistance."));
	

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

	// DamageType
	YuraGameplayTags.DamageType_Fire =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Fire"), FString("Flame Damage (Player's normal attack)."));

	YuraGameplayTags.DamageType_Physic =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Physic"), FString("Warrior melee damage."));

	YuraGameplayTags.DamageType_Lighting =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Lighting"), FString("Lightning damage."));

	YuraGameplayTags.DamageType_Arcane =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Arcane"), FString("Arcane damage."));

	// 添加到数组中，方便一次性全部拿到
	YuraGameplayTags.DamageTypeToResistanceTags.Add(YuraGameplayTags.DamageType_Fire, YuraGameplayTags.Attribute_Resilience_Fire);
	YuraGameplayTags.DamageTypeToResistanceTags.Add(YuraGameplayTags.DamageType_Physic, YuraGameplayTags.Attribute_Resilience_Physic);
	YuraGameplayTags.DamageTypeToResistanceTags.Add(YuraGameplayTags.DamageType_Lighting, YuraGameplayTags.Attribute_Resilience_Lighting);
	YuraGameplayTags.DamageTypeToResistanceTags.Add(YuraGameplayTags.DamageType_Arcane, YuraGameplayTags.Attribute_Resilience_Arcane);
	

	// Effects Tag -- Buff和Debuff
	// 受击僵直
	YuraGameplayTags.Effects_HitReact =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Tag granted when Hit Reacting."));
	
	// Ability Tag
	YuraGameplayTags.Ability_Attack =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Attack"), FString("Tag To Activate Attack Ability."));
	YuraGameplayTags.Ability_Summon =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Summon"), FString("Tag To Activate Summon Ability."));

	// Yura Ability Tag
	YuraGameplayTags.Ability_Fire_FireBolt =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Fire.FireBolt"), FString("Ability Tag for Yura FireBolt."));

	// Coowdown Tag
	YuraGameplayTags.Cooldown_Fire_FireBolt =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"), FString("Cooldown Tag for Yura FireBolt."));

	// CombatSocket Tag
	YuraGameplayTags.CombatSocket_Weapon =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"), FString("Get Socket On Weapon."));
	YuraGameplayTags.CombatSocket_RightHand =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"), FString("Get Socket On RightHand."));
	YuraGameplayTags.CombatSocket_LeftHand =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"), FString("Get Socket On LeftHand."));

	// Montage Tags
	YuraGameplayTags.Montage_Attack_1 =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString("Montage Attack1."));
	YuraGameplayTags.Montage_Attack_2 =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString("Montage Attack2."));
	YuraGameplayTags.Montage_Attack_3 =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString("Montage Attack3."));
	YuraGameplayTags.Montage_Attack_4 =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString("Montage Attack4."));
	YuraGameplayTags.Montage_Summon =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Summon"), FString("Montage Summon."));

}
