// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraFireBlast.h"
#include "Actor/YuraFireBall.h"
#include "AbilitySystem/YuraAbilitySystemLibrary.h"

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

TArray<AYuraFireBall*> UYuraFireBlast::SpawnFireBalls()
{
	TArray<AYuraFireBall*> OutFireBalls;

	// 获取均匀分布的向量
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	TArray<FVector> SpawnVectors =  UYuraAbilitySystemLibrary::EvenlySpacedVectors(Forward, FVector::UpVector, 360.f, NumToSpawn);

	// 玩家的位置
	const FVector PlayerLoaction = GetAvatarActorFromActorInfo()->GetActorLocation();

	for (const FVector& SpawnVector : SpawnVectors)
	{
		// 火球的生成位置，我们不希望直接在玩家位置生成，而是在玩家周围1m处生成
		const FVector SpawnLocation = PlayerLoaction + SpawnVector * 75;
		// 火球的朝向
		const FRotator SpawnRotation = SpawnVector.Rotation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());

		// 延迟生成
		AYuraFireBall* FireBallSpawn = GetWorld()->SpawnActorDeferred<AYuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// 生成伤害效果参数
		FireBallSpawn->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		OutFireBalls.Add(FireBallSpawn);

		FireBallSpawn->FinishSpawning(SpawnTransform);

	}

	return OutFireBalls;
}
