// Copyright Muci Yun.


#include "Actor/YuraFireBall.h"
#include "YuraAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/AudioComponent.h"
#include "GameplayCueManager.h"
#include "YuraGameplayTags.h"

void AYuraFireBall::BeginPlay()
{
	Super::BeginPlay();

	// 这个函数在蓝图实现
	StartFireBallOutingTimeline();
}

void AYuraFireBall::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const AActor* AvatarActor = DamageEffectParams.SourceASC->GetAvatarActor();
	// 避免碰撞到自己，导致异常情况
	if (AvatarActor == OtherActor) return;

	// 避免误伤
	if (!UYuraAbilitySystemLibrary::IsNotFriend(AvatarActor, OtherActor)) return;

	if (HasAuthority())
	{
		// 之前它应该是空的
		DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (DamageEffectParams.TargetASC)
		{
			// 设置冲量
			DamageEffectParams.DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;

			UYuraAbilitySystemLibrary::ApplyDamageEffectByParams(DamageEffectParams);
		}
	}
}

void AYuraFireBall::PreForExplosion(AActor* TargetActor)
{
	const FVector TargetLocation = TargetActor->GetActorLocation();
	// 击退
	KnockbackVector = (TargetLocation - GetInstigator()->GetActorLocation()).GetSafeNormal() * KnockbackForceMagnitude;

	// 径向伤害中心点就是它的位置
	RadialCenterLocation = GetActorLocation();

	DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	DamageEffectParams.DeathImpulse = (TargetLocation - GetInstigator()->GetActorLocation()).GetSafeNormal() * DamageEffectParams.DeathImpulseMagnitude;

	DamageEffectParams.KnockbackChance = KnockbackChance;
	DamageEffectParams.KnockbackForceMagnitude = KnockbackForceMagnitude;
	DamageEffectParams.KnockbackVector = KnockbackVector;
	

	if (bIsRadialDamge)
	{
		DamageEffectParams.bIsRadialDamge = bIsRadialDamge;

		DamageEffectParams.RadialCenterLocation = RadialCenterLocation;
		DamageEffectParams.RadialOuterRadius = RadialOuterRadius;
		DamageEffectParams.RadialInnerRadius = RadialInnerRadius;

		// 爆炸的话，伤害会有所增加
		DamageEffectParams.AbilityBaseDamage *= ExplosionScale;
	}
	
}

void AYuraFireBall::OnHit()
{
	// 停止播放飞行的声音
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent->DestroyComponent();
	}

	if (GetInstigator())
	{
		FGameplayCueParameters ExplosionParams;
		ExplosionParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetInstigator(), FYuraGameplayTags::Get().GameplayCue_FireBlast, ExplosionParams);
	}
	

	bHit = true;
}
