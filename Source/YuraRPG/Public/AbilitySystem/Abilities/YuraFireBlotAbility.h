// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/YuraProjectileSpell.h"
#include "YuraFireBlotAbility.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraFireBlotAbility : public UYuraProjectileSpell
{
	GENERATED_BODY()

public:

	// 获取当前描述
	virtual FString GetCurrentLevelDescription(int Level) override;
	// 获取下一级的描述
	virtual FString GetNextLevelDescription(int Level) override;

	// 专门用于生成火球的函数--随着等级提升，可以发射多个火球
	UFUNCTION(BlueprintCallable, Category = "FireBoltAbility")
	void SpawnFireBolts(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, AActor* HomeingTarget, float PitchOverride = 0.f);

protected:
	// 生成的角度范围
	UPROPERTY(EditDefaultsOnly, Category = "FireBoltAbility")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBoltAbility|Homing")
	float MinHomingAccelerationMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBoltAbility|Homing")
	float MaxHomingAccelerationMagnitude = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBoltAbility|Homing")
	bool bIsHomingProjectile = true;
	
};
