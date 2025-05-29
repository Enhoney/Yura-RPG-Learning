// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraFireBlast.h"

FString UYuraFireBlast::GetCurrentLevelDescription(int Level)
{
	// 获取火焰元素伤害，因为玩家这个就是火球，只会造成火焰元素伤害
	const int32 BaseDamageCause = GetBaseDamageTyped(DamageTypeTag, Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		/** Title*/
		"<Title>Fire Blast</>\n\n"
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
		"<Default>Launch %d Fire balls in all directions, "
		"each coming back and exploding upon return, causing </>"
		"<Damage>%d</>""<Type> radial fire </>"
		"<Default> damage with a chance to burn.</>"),
		Level, BaseDamageCause, ManaCost, Cooldown, NumToSpawn, BaseDamageCause);

}

FString UYuraFireBlast::GetNextLevelDescription(int Level)
{
	// 获取火焰元素伤害，因为玩家这个就是火球，只会造成火焰元素伤害
	const int32 BaseDamageCause = GetBaseDamageTyped(DamageTypeTag, Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

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
		"<Default>Launch %d Fire balls in all directions, "
		"each coming back and exploding upon return, causing </>"
		"<Damage>%d</>""<Type> radial fire </>"
		"<Default> damage with a chance to burn.</>"),
		Level, BaseDamageCause, ManaCost, Cooldown, NumToSpawn, BaseDamageCause);
}
