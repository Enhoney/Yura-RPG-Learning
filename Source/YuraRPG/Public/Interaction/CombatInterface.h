// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "GameplayTagContainer.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TaggedMontage")
	class UAnimMontage* AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TaggedMontage")
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TaggedMontage")
	FGameplayTag CombatSocketTag;

	// 命中音效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TaggedMontage")
	class USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class YURARPG_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 不是必须重写的
	// 获取角色等级
	virtual int32 GetCharacterLevel();

	// 获取WeaponTipSocketLocation
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetFireSocketLocation(const FGameplayTag& CombatSocketTag);

	// 设置Wrap位置朝向
	virtual void SetWarpTargetFacing(const FVector& TargetLocation);

	// 获取受击动画
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UAnimMontage* GetHitReactAnimMontage();

	virtual void Die() = 0;

	// 敌人使用，获取和设置攻击目标
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;

	// Character是否死亡
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead() const;

	// 如果它上面有ASC，就返回它自己
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetAvator();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FTaggedMontage> GetAttackMontages() const;

	// 获取Character上面的受击效果
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UNiagaraSystem* GetImpactEffect() const;
	
	// 通过MontageTag找到配置的命中音效
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FTaggedMontage GetTaggedMontageByMontageTag(const FGameplayTag& MontageTag) const;
};
