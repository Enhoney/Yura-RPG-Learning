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

class UBehaviorTree;
class AYuraAIController;

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

	virtual void Die() override;
	/** Combat Interface end*/

	// 手动调用获取初始值
	UFUNCTION(BlueprintCallable)
	void CallInitHealthValue();

	// Hit React--当HitReact tag被添加时，执行回调，用于激活相应GA，播放受击动画
	void OnHitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount);

	// 被Controlelr掌控的时候--敌人类在这里获取AIController
	virtual void PossessedBy(AController* NewController) override;

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

	// 是否处于受击状态
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	// 记录移动速度
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 0.f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Default Class")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health Bar")
	TObjectPtr<UWidgetComponent> HealthBar;

	// 类别
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Default Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	// 受击动画蒙太奇
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float LifeSpawnOnDeath = 5.f;

	// 使用的行为树
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Controller")
	TObjectPtr<UBehaviorTree> EnemyBehaviorTree;

	// 记录使用的AIController，不暴露给蓝图
	UPROPERTY()
	TObjectPtr<AYuraAIController> EnemyAIController;
};
