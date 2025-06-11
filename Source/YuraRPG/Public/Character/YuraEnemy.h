// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Character/YuraCharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/CombatInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "YuraEnemy.generated.h"

class UWidgetComponent;

class UBehaviorTree;
class AYuraAIController;

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraEnemy : public AYuraCharacterBase, public IHighlightInterface
{
	GENERATED_BODY()

public:

	AYuraEnemy();

	/** Highlight Interface start*/
	virtual void HighlightActor_Implementation() override;
	virtual void UnhighlightActor_Implementation() override;
	/** Highlight Interface end*/

	/** Combat Interface start*/
	virtual int32 GetCharacterLevel() const override;

	virtual void Die(const FVector& InDeathImpulse) override;

	virtual AActor* GetCombatTarget_Implementation() const override;

	virtual void SetCombatTarget_Implementation(AActor* TargetActor) override;
	/** Combat Interface end*/

	// 手动调用获取初始值
	UFUNCTION(BlueprintCallable)
	void CallInitHealthValue();

	// Hit React--当HitReact tag被添加时，执行回调，用于激活相应GA，播放受击动画
	void OnHitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount);

	// 被Controlelr掌控的时候--敌人类在这里获取AIController
	virtual void PossessedBy(AController* NewController) override;

	FORCEINLINE  void SetCharacterLevel(int32 InLevel) { CharacterLevel = InLevel; }

protected:
	void BeginPlay() override;

	virtual void InitializeDefaultAttributes() const override;

	void HandleStunDebuffInAnim(const FGameplayTag InDebuffTag, int32 NewCount) override;

	// 生成战利品
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLootTiers();

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Default Class")
	int32 CharacterLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health Bar")
	TObjectPtr<UWidgetComponent> HealthBar;

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

	// 攻击的目标
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

};
