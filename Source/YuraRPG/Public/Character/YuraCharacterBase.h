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

protected:
	virtual void BeginPlay() override;

	void InitializeDefaultAttributes() const;


private:

	virtual void InitAbilityActorInfo();

	void ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& GEForAttributes, float Level = 1.0f) const;

protected:

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

};
