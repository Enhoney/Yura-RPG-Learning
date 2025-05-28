// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraArcaneShardsSpell.h"

FString UYuraArcaneShardsSpell::GetCurrentLevelDescription(int Level)
{
	const int32 BaseDamageCause = GetBaseDamageTyped(DamageTypeTag, Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			/** Title*/
			"<Title>Arcane Shards</>\n\n"
			/** Level*/
			"<Default>CurrentLevel:</>\t<Level>%d</>\n\n"
			/** DamageType*/
			"<Small>DamageType: </><Type>Arcane</>\n"
			/** BaseDamage*/
			"<Small>BaseDamage: </><Damage>%d</>\n"
			/** ManaCost*/
			"<Small>ManaCost: </><Cost>%.2f</>\n"
			/** Cooldown*/
			"<Small>Cooldown: </><Cooldown>%.2fs</>\n\n"
			/** Details*/
			"<Default>Select a position and generate a crystal at that position, dealing area damage to enemies within the range, with </>"
			"<Damage>%d</>""<Type> arcane </>"
			"<Default> damage at the center.</>"),
			Level, BaseDamageCause, ManaCost, Cooldown, BaseDamageCause);
	}
	else
	{

		return FString::Printf(TEXT(
			/** Title*/
			"<Title>Arcane Shards</>\n\n"
			/** Level*/
			"<Default>CurrentLevel:</>\t<Level>%d</>\n\n"
			/** DamageType*/
			"<Small>DamageType: </><Type>Arcane</>\n"
			/** BaseDamage*/
			"<Small>BaseDamage: </><Damage>%d</>\n"
			/** ManaCost*/
			"<Small>ManaCost: </><Cost>%.2f</>\n"
			/** Cooldown*/
			"<Small>Cooldown: </><Cooldown>%.2fs</>\n\n"
			/** Details*/
			"<Default>Select a position and generate a crystal at that position, dealing area damage to enemies within the range, with </>"
			"<Damage>%d</>""<Type> arcane </>"
			"<Default> damage at the center.And randomly generate %d crystals around.</>"),
			Level, BaseDamageCause, ManaCost, Cooldown, BaseDamageCause, GetAbilityLevel() - 1);
	}
}

FString UYuraArcaneShardsSpell::GetNextLevelDescription(int Level)
{
	const int32 BaseDamageCause = GetBaseDamageTyped(DamageTypeTag, Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		/** Title*/
		"<Title>NextLevel:    %d</>\n\n"
		/** DamageType*/
		"<Small>DamageType: </><Type>Arcane</>\n"
		/** BaseDamage*/
		"<Small>BaseDamage: </><Damage>%d</>\n"
		/** ManaCost*/
		"<Small>ManaCost: </><Cost>%.2f</>\n"
		/** Cooldown*/
		"<Small>Cooldown: </><Cooldown>%.2fs</>\n\n"
		/** Details*/
		"<Default>Select a position and generate a crystal at that position, dealing area damage to enemies within the range, with </>"
		"<Damage>%d</>""<Type> arcane </>"
		"<Default> damage at the center.And randomly generate %d crystals around.</>"),
		Level, BaseDamageCause, ManaCost, Cooldown, BaseDamageCause, GetAbilityLevel());
}
