// Copyright Muci Yun.


#include "Actor/YuraProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "YuraRPG.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

#include "YuraAbilitySystemLibrary.h"

#include "Net/UnrealNetwork.h"

AYuraProjectile::AYuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	// 开启复制
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	// 仅查询
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 设置ObjectType
	Sphere->SetCollisionObjectType(ECC_Projectile);
	// 仅对于指定通道重叠
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	// 默认速度
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	// 忽略重力
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

}

void AYuraProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AYuraProjectile, DamageEffectParams, COND_InitialOnly);
}

void AYuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);

	AudioComponent = UGameplayStatics::SpawnSoundAttached(FlyingtSound, GetRootComponent());
}

void AYuraProjectile::Destroyed()
{
	// 这个时候，还没有销毁呢
	// 如果这个时候在客户端，并且还没有碰撞到，就手动播放效果
	if (!HasAuthority() && !bHit)
	{
		OnHit();
	}

	Super::Destroyed();
}

void AYuraProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	const AActor* AvatarActor = DamageEffectParams.SourceASC->GetAvatarActor();
	// 避免碰撞到自己，导致异常情况
	if (AvatarActor == OtherActor) return;

	// 避免误伤
	if (!UYuraAbilitySystemLibrary::IsNotFriend(AvatarActor, OtherActor)) return;

	OnHit();

	if (HasAuthority())
	{
		// 之前它应该是空的
		DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (DamageEffectParams.TargetASC)
		{
			// 设置冲量
			DamageEffectParams.DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;

			// 如果成功触发击退了
			if (FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance * 100)
			{
				// 计算击退向量
				FRotator ActorMoveRotation = ProjectileMovementComponent->Velocity.Rotation();
				ActorMoveRotation.Pitch = 45.f;
				FVector KnockDirection = ActorMoveRotation.Vector().GetSafeNormal();
				// 最终得到击退的向量
				DamageEffectParams.KnockbackVector = KnockDirection * DamageEffectParams.KnockbackForceMagnitude;
			}

			UYuraAbilitySystemLibrary::ApplyDamageEffectByParams(DamageEffectParams);
		}
		// 无论如何都销毁，但只有命中敌人的时候才施加效果
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

void AYuraProjectile::OnHit()
{
	// 停止播放飞行的声音
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent->DestroyComponent();
	}

	// 在指定位置播放音效
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactNiagaraEffect, GetActorLocation());
	bHit = true;
}

