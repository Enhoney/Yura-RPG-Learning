// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "YuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
  	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UYuraAttributeSet();

	// 为那些需要复制的属性注册属性同步
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	ATTRIBUTE_ACCESSORS(UYuraAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UYuraAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UYuraAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UYuraAttributeSet, MaxMana);

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

protected:
	// 血量
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Primary Attribute")
	FGameplayAttributeData Health;
	// 最大血量
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Primary Attribute")
	FGameplayAttributeData MaxHealth;

	// 魔法值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Primary Attribute")
	FGameplayAttributeData Mana;
	// 最大魔法值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Primary Attribute")
	FGameplayAttributeData MaxMana;
	
};
