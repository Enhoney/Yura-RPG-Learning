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
		if(AudioComponent) AudioComponent->Stop();
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactNiagaraEffect, GetActorLocation());
	}

	Super::Destroyed();
}

void AYuraProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 避免碰撞到自己，导致异常情况
	if (DamageEffectSpecHandle.Data.IsValid() && DamageEffectSpecHandle.Data->GetEffectContext().GetEffectCauser() == OtherActor)
	{
		return;
	}

	// 如果是在服务器上，就销毁这个发射物体
	// 这里可能存在这样一种情况--客户端还没有播放音效和特效，服务端已经执行到销毁，声音和Niagara只在客户端上有
	// 这会导致客户端没有任何反馈，发射物就直接消失了
	// 所以需要做一些操作来保证
	if (HasAuthority())
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (TargetASC)
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		Destroy();
	}
	else
	{
		// 停止播放飞行的声音
		if(AudioComponent) AudioComponent->Stop();

		// 在指定位置播放音效
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactNiagaraEffect, GetActorLocation());
		bHit = true;
	}
}

