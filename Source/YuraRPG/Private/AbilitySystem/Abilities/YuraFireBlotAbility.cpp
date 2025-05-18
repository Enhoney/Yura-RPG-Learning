// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraFireBlotAbility.h"
#include "YuraGameplayTags.h"

FString UYuraFireBlotAbility::GetCurrentLevelDescription(int Level)
{
	// 获取火焰元素伤害，因为玩家这个就是火球，只会造成火焰元素伤害
	const int32 BaseDamageCause = GetBaseDamageTyped(FYuraGameplayTags::Get().DamageType_Fire, Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			/** Title*/
			"<Title>Fire Bolt</>\n\n"
			/** Level*/
			"<Default>CurrentLevel:</>\t<Level>%d</>\n\n"
			/** DamageType*/
			"<Small>DamageType: </><Type>Fire</>\n"
			/** BaseDamage*/
			"<Small>BaseDamage: </><Damage>%d</>\n"
			/** ManaCost*/
			"<Small>ManaCost: </><Cost>%.2f</>\n"
			/** Cooldown*/
			"<Small>Cooldown: </><Cooldown>%.2fs</>\n\n"
			/** Details*/
			"<Default>Fire a fireball in the designated direction, dealing </>"
			"<Damage>%d</>""<Type> fire </>"
			"<Default> base damage with a chance to burn</>"),
			Level, BaseDamageCause, ManaCost, Cooldown, BaseDamageCause);
	}
	else
	{
		// 最多只会发射NumToSpawn个火球，实际数量和等级挂钩
		const int32 FireBoltNum = FMath::Min(Level, MaxNumToSpawn);

		return FString::Printf(TEXT(
			/** Title*/
			"<Title>Fire Bolt</>\n\n"
			/** Level*/
			"<Default>CurrentLevel:</>\t<Level>%d</>\n\n"
			/** DamageType*/
			"<Small>DamageType: </><Type>Fire</>\n"
			/** BaseDamage*/
			"<Small>BaseDamage: </><Damage>%d</>\n"
			/** ManaCost*/
			"<Small>ManaCost: </><Cost>%.2f</>\n"
			/** Cooldown*/
			"<Small>Cooldown: </><Cooldown>%.2fs</>\n\n"
			/** Details*/
			"<Default>Fire %d fireballs in the designated direction, dealing </>"
			"<Damage>%d</>""<Type> fire </>"
			"<Default> base damage with a chance to burn.</>"),
			Level, BaseDamageCause, ManaCost, Cooldown, FireBoltNum, BaseDamageCause);
	}
}

FString UYuraFireBlotAbility::GetNextLevelDescription(int Level)
{
	const int32 BaseDamageCause = GetBaseDamageTyped(FYuraGameplayTags::Get().DamageType_Fire, Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	// 最多只会发射NumToSpawn个火球，实际数量和等级挂钩
	const int32 FireBoltNum = FMath::Min(Level, MaxNumToSpawn);
	return FString::Printf(TEXT(
		/** Title*/
		"<Title>NextLevel:    %d</>\n\n"
		/** DamageType*/
		"<Small>DamageType: </><Type>Fire</>\n"
		/** BaseDamage*/
		"<Small>BaseDamage: </><Damage>%d</>\n"
		/** ManaCost*/
		"<Small>ManaCost: </><Cost>%.2f</>\n"
		/** Cooldown*/
		"<Small>Cooldown: </><Cooldown>%.2fs</>\n\n"
		/** Details*/
		"<Default>Fire %d fireballs in the designated direction, dealing </>"
		"<Damage>%d</>""<Type> fire </>"
		"<Default> base damage with a chance to burn.</>"),
		Level, BaseDamageCause, ManaCost, Cooldown, FireBoltNum, BaseDamageCause);
}

