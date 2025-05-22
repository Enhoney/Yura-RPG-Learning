// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
class UAbilitySystemComponent;

// ASC初始化完成回调
DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCInitializedSignature, UAbilitySystemComponent*);

// 死亡回调
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDeathSignature, AActor*, DeadActor);

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
	virtual int32 GetCharacterLevel() const;

	// 获取WeaponTipSocketLocation
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetFireSocketLocation(const FGameplayTag& CombatSocketTag);

	// 设置Wrap位置朝向
	virtual void SetWarpTargetFacing(const FVector& TargetLocation);

	// 获取受击动画
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UAnimMontage* GetHitReactAnimMontage();

	virtual void Die(const FVector& InDeathImpulse) = 0;

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
	AActor* GetAvatar();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FTaggedMontage> GetAttackMontages() const;

	// 获取Character上面的受击效果
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UNiagaraSystem* GetImpactEffect() const;
	
	// 通过MontageTag找到配置的命中音效
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FTaggedMontage GetTaggedMontageByMontageTag(const FGameplayTag& MontageTag) const;

	// 获取随从数量
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetMinionCount() const;

	// 增加随从数量
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddMinionCount(int32 Num);

	// 减少随从数量
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReduceMinionCount(int32 Num);

	// 获取角色类别
	virtual ECharacterClass GetCharacterClass() const;

	// 获取FOnASCInitializedSignature
	virtual FOnASCInitializedSignature& GetOnASCInitializedDelegate() = 0;

	// 获取死亡回调
	virtual FOnActorDeathSignature& GetOnActorDeathDelegate() = 0;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCastShockingLoop(bool bInShockingLoop);
};
