// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/YuraDamageGameplayAbility.h"
#include "YuraProjectileSpell.generated.h"

class AYuraProjectile;
class UGameplayEffect;

struct FGameplayTag;

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraProjectileSpell : public UYuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	//定义了 能力激活之后我们要做的事
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;


	// 蓝图可调用，生成子弹
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag);

protected:
	// 发射物的类
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AYuraProjectile> ProjectileClass;

	// 一次释放技能最多发射的数量
	UPROPERTY(EditDefaultsOnly)
	int32 MaxNumToSpawn = 1;

};
