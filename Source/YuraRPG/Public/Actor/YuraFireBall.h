// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Actor/YuraProjectile.h"
#include "YuraFireBall.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraFireBall : public AYuraProjectile
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void StartFireBallOutingTimeline();

protected:

	virtual void BeginPlay() override;
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// PreExplosive
	UFUNCTION(BlueprintCallable)
	void PreForExplosion(AActor* TargetActor);

	virtual void OnHit() override;


protected:
	// 爆炸伤害使用的参数

	// 击退力度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive")
	float KnockbackForceMagnitude = 400.f;

	// 击退几率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive")
	float KnockbackChance = 1.0f;

	// 击退向量--方向和力度
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackVector = FVector::ZeroVector;

	/** 径向衰减范围伤害*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive")
	bool bIsRadialDamge = true;	// 是否为径向伤害

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive")
	float RadialInnerRadius = 50.f;	// 中心半径

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive")
	float RadialOuterRadius = 100.f;	// 伤害范围

	UPROPERTY(BlueprintReadWrite, Category = "Explosive")
	FVector RadialCenterLocation = FVector();	// 伤害中心点

	// 爆炸伤害增幅
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive")
	float ExplosionScale = 1.5f;
	
};
