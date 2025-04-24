// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class USoundBase;
class UAudioComponent;

UCLASS()
class YURARPG_API AYuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AYuraProjectile();

	// 获取移动组件--我们可能需要设置它的速度啥的
	FORCEINLINE UProjectileMovementComponent* GetMovementComponent() { return ProjectileMovementComponent;}

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	// 重叠回调
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	// 球体碰撞
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	TObjectPtr<USphereComponent> Sphere;

	// 发射物移动组件
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	// 命中的效果
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TObjectPtr<UNiagaraSystem> ImpactNiagaraEffect;

	// 命中的音效
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TObjectPtr<USoundBase> FlyingtSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;

	bool bHit = false;
};
