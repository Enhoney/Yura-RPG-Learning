// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/YuraWidgetController.h"
#include "YuraGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"


// 用于显示技能点
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellPointChangedSignature, int32, NewSpellPoints);

// 用于更新两个按钮的可用状态
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellMenuButonSignature, bool, bSpellPointButttonEnable, bool, bEquipButtonEnable, FString, CurDescription, FString, NextLevelDescription);

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();	// 选中技能的AbilityTag
	FGameplayTag StatusTag = FGameplayTag();		// 选中技能的状态

};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class YURARPG_API USpellMenuWidgetController : public UYuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependiencies() override;

	UFUNCTION(BlueprintCallable, Category = "SpellMenu")
	void HandleSpellPointAndEquipButton(const FGameplayTag& InAbilityTag);

	// 在技能菜单销毁时调用--清空现在选择的Ability
	UFUNCTION(BlueprintCallable, Category = "SpellMenu")
	void ClearSelectedAbility();

	// 花费技能点按钮按下事件
	UFUNCTION(BlueprintCallable, Category = "SpellMenu")
	void OnSpellPointButtonPressed();

private:
	// 技能点或者状态改变时，处理
	void HandleButtonEnableOnChanged(int32 NewSpellPoint, const FGameplayTag& NewStatusTag);



public:

	UPROPERTY(BlueprintAssignable, Category = "Ability|Spell")
	FSpellPointChangedSignature OnSpellPointChangedDelegate;

	// 控制两个按钮的可用与否
	UPROPERTY(BlueprintAssignable, Category = "SpellMenu")
	FSpellMenuButonSignature OnSpellMenuButtonDelegate;

private:
	FSelectedAbility SelectedAbility;
	
	int32 CurrentSpellPoint = 0;
};
