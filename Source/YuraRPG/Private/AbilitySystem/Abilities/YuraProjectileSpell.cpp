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

	// 在服务器生成
	const bool bServer = HasAuthority(&ActivationInfo);
	if(!bServer)
	{
		return;
	}
	check(ProjectileClass);
	
	// 使用接口来获取SocketLocation
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SpawnLocation = CombatInterface->GetFireSocketLocation();
		// TODO: 设置旋转

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);
		// 延迟生成，因为我们还需要设置一些参数，毕竟它是有移动组件的，一旦生成完毕就开始移动了
		/** 参数说明，Owner和Instagitor可以不一样，但是Instagitor一定是一个Pawn*/
		AYuraProjectile* Projectile =  GetWorld()->SpawnActorDeferred<AYuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		//TODO: 我们要在这里给他一个GameplayEffectSpec

		Projectile->FinishSpawning(SpawnTransform);
	}
	
}
