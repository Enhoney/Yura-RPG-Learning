// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemInterface.h"
#include "YuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

class UMaterialInstance;

UCLASS(Abstract)
class YURARPG_API AYuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AYuraCharacterBase();

public:
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 自己定义的方法，方便获取属性
	UAttributeSet* GetAttributeSet() const;

	/** ConbatInterface start*/
	virtual FVector GetFireSocketLocation_Implementation(const FGameplayTag& MontageTag) override;

	virtual void SetWarpTargetFacing(const FVector& TargetLocation) override;

	virtual void Die() override;

	virtual bool IsDead_Implementation() const override;

	virtual AActor* GetAvator_Implementation() override;

	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() const override;
	/** ConbatInterface end*/

	// 处理死亡动画，玩家死亡逻辑和敌人的是不一样的，所以需要分开来做
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	/** 溶解--创建动态材质，并调用开始溶解的函数*/ 
	void Disslove();

	// 获取武器骨骼网格体
	UFUNCTION(BlueprintPure)
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return Weapon; }

protected:
	virtual void BeginPlay() override;

	virtual void InitializeDefaultAttributes() const;

	virtual void AddCharacterAbilities();

	/** 开始溶解，使用TimeLine，在蓝图比较好实现*/
	UFUNCTION(BlueprintImplementableEvent)
	void StartMeshDissolveTimeLine(UMaterialInstanceDynamic* DissolveMatDynamic);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeLine(UMaterialInstanceDynamic* DissolveMatDynamic);

private:

	virtual void InitAbilityActorInfo();

	void ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& GEForAttributes, float Level = 1.0f) const;

protected:

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UMotionWarpingComponent> MotionWarping;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WarpingTargetName = FName();

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultSedcondaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	// 溶解材质实例
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UMaterialInstance> DissolveEffectMatForMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UMaterialInstance> DissolveEffectMatForWeapon;

	// AttackMontage And Tag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

	// Map Montage and SocketName
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TMap<FGameplayTag, FName> MapMontageToFireSocket;

private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesGrantIngOnStart;

	bool bIsDead = false;

};
