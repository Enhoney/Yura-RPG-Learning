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
	virtual FVector GetFireSocketLocation() override;

	virtual void SetWarpTargetFacing(const FVector& TargetLocation) override;
	/** ConbatInterface end*/

protected:
	virtual void BeginPlay() override;

	virtual void InitializeDefaultAttributes() const;

	virtual void AddCharacterAbilities();


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

	// 武器尖端SocketName
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

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

private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesGrantIngOnStart;

};
