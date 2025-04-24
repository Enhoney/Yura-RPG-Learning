// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraProjectileSpell.h"
#include "Actor/YuraProjectile.h"

#include "Interaction/CombatInterface.h"

void UYuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UYuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	// 在服务器生成
	const bool bServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bServer)
	{
		return;
	}
	check(ProjectileClass);

	// 使用接口来获取SocketLocation
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SpawnLocation = CombatInterface->GetFireSocketLocation();
		// 设置旋转
		FRotator SpawnRotation = (ProjectileTargetLocation - SpawnLocation).Rotation();
		// 将俯仰角归零，只能水平移动
		SpawnRotation.Pitch = 0;
		// TODO：如果希望做一个抛物线，可以将这个俯仰角加25左右，然后开启重力
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());
		// 延迟生成，因为我们还需要设置一些参数，毕竟它是有移动组件的，一旦生成完毕就开始移动了
		/** 参数说明，Owner和Instagitor可以不一样，但是Instagitor一定是一个Pawn*/
		AYuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AYuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		//TODO: 我们要在这里给他一个GameplayEffectSpec

		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UYuraProjectileSpell::SetWarpingTargetFacingFromLocation(const FVector& ProjectileTargetLocation)
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		CombatInterface->SetWarpTargetFacing(ProjectileTargetLocation);
	}
}
