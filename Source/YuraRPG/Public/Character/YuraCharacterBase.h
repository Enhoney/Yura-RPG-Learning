// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "YuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class YURARPG_API AYuraCharacterBase : public ACharacter, public IAbilitySystemInterface
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

private:

	virtual void InitAbilityActorInfo();


protected:

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

};
