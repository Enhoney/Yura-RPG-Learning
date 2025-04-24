// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/YuraGameplayAbility.h"
#include "YuraProjectileSpell.generated.h"

class AYuraProjectile;

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraProjectileSpell : public UYuraGameplayAbility
{
	GENERATED_BODY()

protected:
	//定义了 能力激活之后我们要做的是事
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	// 蓝图可调用，生成子弹
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	// 设置WarpingTargetFacing
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetWarpingTargetFacingFromLocation(const FVector& ProjectileTargetLocation);

protected:
	// 发射物的类
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AYuraProjectile> ProjectileClass;
	
};
