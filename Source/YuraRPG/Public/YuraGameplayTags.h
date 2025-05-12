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

	// Ressilience For DamageTypes
	// 对各种伤害的抗性
	FGameplayTag Attribute_Resilience_Fire;
	FGameplayTag Attribute_Resilience_Physic;
	FGameplayTag Attribute_Resilience_Lighting;
	FGameplayTag Attribute_Resilience_Arcane;

	// Meta属性Tag
	FGameplayTag Attribute_Meta_IncomingExp;

	// Ability Input Tags
	// 这些tag和实际的按键不一定是完全对应的
	FGameplayTag InputTag_LMB;	// 鼠标左键
	FGameplayTag InputTag_RMB;	// 鼠标右键
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	// 用于伤害计算SetCallByCaller的Tag
	// 伤害类型
	FGameplayTag DamageType_Fire;	// 火球
	FGameplayTag DamageType_Physic;	// 战士近战伤害
	FGameplayTag DamageType_Lighting;	// 闪电伤害
	FGameplayTag DamageType_Arcane;		// 奥义伤害

	// 存储所有伤害类型抗性类型的Map
	TMap<FGameplayTag /**DamageTpype*/, FGameplayTag /** ResistanceType*/> DamageTypeToResistanceTags;

	// 各种Buff和Debuff
	// 受击僵直
	FGameplayTag Effects_HitReact;

	// 用于激活技能的tag
	FGameplayTag Ability_Attack;
	FGameplayTag Ability_Summon;

	// 玩家的技能Tag
	FGameplayTag Ability_Fire_FireBolt;

	// 技能冷却时间的tag
	FGameplayTag Cooldown_Fire_FireBolt;

	// 和Socket关联的Tag--攻击时用于生成Trace的位置
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;

	// 攻击蒙太奇的Tag
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	FGameplayTag Montage_Summon;

private:
	static FYuraGameplayTags YuraGameplayTags;
};
