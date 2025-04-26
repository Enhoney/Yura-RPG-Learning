// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Character/YuraCharacterBase.h"
#include "EnemyInterface.h"
#include "Interaction/CombatInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "YuraEnemy.generated.h"

class UWidgetComponent;

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraEnemy : public AYuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:

	AYuraEnemy();

	/** Enemy Interface start*/
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
	/** Enemy Interface end*/

	/** Combat Interface start*/
	virtual int32 GetCharacterLevel() override;
	/** Combat Interface end*/

	// 手动调用获取初始值
	UFUNCTION(BlueprintCallable)
	void CallInitHealthValue();

protected:
	void BeginPlay() override;

	virtual void InitializeDefaultAttributes() const override;

private:

	virtual void InitAbilityActorInfo() override;
public:
	UPROPERTY(BlueprintReadOnly)
	bool bHighlight = false;

	// 角色类作为WidgetController，执行广播
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Default Class")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health Bar")
	TObjectPtr<UWidgetComponent> HealthBar;

	// 类别
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Default Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
};
