// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraFireBlotAbility.h"
#include "YuraGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystem/YuraAbilitySystemLibrary.h"
#include "Actor/YuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UYuraFireBlotAbility::GetCurrentLevelDescription(int Level)
{
	// 获取火焰元素伤害，因为玩家这个就是火球，只会造成火焰元素伤害
	const int32 BaseDamageCause = GetBaseDamageTyped(DamageTypeTag, Level);
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
	const int32 BaseDamageCause = GetBaseDamageTyped(DamageTypeTag, Level);
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

void UYuraFireBlotAbility::SpawnFireBolts(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, AActor* HomeingTarget, float PitchOverride)
{
	// 在服务器生成
	const bool bServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bServer)
	{
		return;
	}
	check(ProjectileClass);

	// 使用接口来获取SocketLocation
	const FVector SocketLocation = ICombatInterface::Execute_GetFireSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);

	// 生成的数量
	const int32 NumToSpawn = FMath::Min(MaxNumToSpawn, GetAbilityLevel());

	// 向上的角度偏移
	FRotator PitchOverrideRotation = FRotator(PitchOverride, 0.f, 0.f);

	// 得到一系列方向向量
	TArray<FVector> DirectionsToSpawn = UYuraAbilitySystemLibrary::EvenlySpacedVectors((ProjectileTargetLocation - SocketLocation).GetSafeNormal(),
		FVector::UpVector, ProjectileSpread, NumToSpawn);

	for (FVector& DirectionVector : DirectionsToSpawn)
	{
		// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + DirectionVector * 400, 2.f, FLinearColor::Green, 5.f);

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation + DirectionVector * 50.f);
		SpawnTransform.SetRotation((DirectionVector.Rotation() + PitchOverrideRotation).Quaternion());
		// 延迟生成，因为我们还需要设置一些参数，毕竟它是有移动组件的，一旦生成完毕就开始移动了
		/** 参数说明，Owner和Instagitor可以不一样，但是Instagitor一定是一个Pawn*/
		AYuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AYuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// 生成Params--现在targetActor肯定是空的
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		// HomingTarget
		if (HomeingTarget)
		{
			if (HomeingTarget->Implements<UCombatInterface>())
			{
				// 自动归航
				Projectile->GetMovementComponent()->HomingTargetComponent = HomeingTarget->GetRootComponent();
			}
			else
			{
				// 如果点击的是地板呢？我们依然想让他归航，但是并不想让它归航到地板或者墙壁的RootComponent
				// 那么做法就是动态创建一个SceneComponent，并让它在Projectile上持有，这样，在Projectile销毁的时候，它也可以回收
				Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
				Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
				Projectile->GetMovementComponent()->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
			}
			// 归航加速度
			Projectile->GetMovementComponent()->HomingAccelerationMagnitude = FMath::FRandRange(MinHomingAccelerationMagnitude, MaxHomingAccelerationMagnitude);

			// 开启归航
			Projectile->GetMovementComponent()->bIsHomingProjectile = bIsHomingProjectile;
		}
		

		Projectile->FinishSpawning(SpawnTransform);
	}
}

