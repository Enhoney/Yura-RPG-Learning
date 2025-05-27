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
	// 被动技能的InputTag
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	// 用于伤害计算SetCallByCaller的Tag
	// 伤害类型
	FGameplayTag DamageType_Fire;	// 火球
	FGameplayTag DamageType_Physic;	// 战士近战伤害
	FGameplayTag DamageType_Lighting;	// 闪电伤害
	FGameplayTag DamageType_Arcane;		// 奥义伤害

	// 存储所有伤害类型抗性类型的Map
	TMap<FGameplayTag /**DamageTpype*/, FGameplayTag /** ResistanceType*/> DamageTypeToResistanceTags;

	// Debuff
	FGameplayTag Debuff_Fire_Burn;	// 灼烧
	FGameplayTag Debuff_Physic_Custom;	// TODO
	FGameplayTag Debuff_Lighting_Stun;	// 眩晕
	FGameplayTag Debuff_Arcane_Custom;	// TODO

	// 存储所有伤害类型与Debuff类型的Map
	TMap<FGameplayTag /**DamageTpype*/, FGameplayTag /** DebuffType*/> DamageTypeToDebuff;

	// Debuff的参数
	FGameplayTag DebuffParam_Chance;
	FGameplayTag DebuffParam_Duration;
	FGameplayTag DebuffParam_Frequency;
	FGameplayTag DebuffParam_BaseDamage;

	// 受击僵直
	FGameplayTag Effects_HitReact;

	// 技能类型
	FGameplayTag Ability_Type_Offensive;
	FGameplayTag Ability_Type_Passive;
	FGameplayTag Ability_Type_None;

	// 技能状态
	FGameplayTag Ability_Status_Locked;
	FGameplayTag Ability_Status_Eligible;
	FGameplayTag Ability_Status_Unlocked;
	FGameplayTag Ability_Status_Equipped;

	// 用于激活技能的tag--敌人
	FGameplayTag Ability_Attack;
	FGameplayTag Ability_Summon;

	// 玩家的技能Tag
	FGameplayTag Ability_Fire_FireBolt;
	FGameplayTag Ability_Lightning_Electrocute;
	FGameplayTag Ability_Arcane_ArcaneShards;

	// 被动技能
	FGameplayTag Ability_Passive_HaloOfProtection;
	FGameplayTag Ability_Passive_HealthSiphon;
	FGameplayTag Ability_Passive_ManaSiphon;

	// 技能冷却时间的tag
	FGameplayTag Cooldown_Fire_FireBolt;
	FGameplayTag Cooldown_Lightning_Electrocute;

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

	// PlayerBlockTag--用于禁用PlayerController中的某些行为
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;

private:
	static FYuraGameplayTags YuraGameplayTags;
};
