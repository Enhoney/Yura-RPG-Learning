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

// 用于在点击装备按钮之后选择提示框动画
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitforEquipSelectSignature, const FGameplayTag&, AbilityTypeTag);

// 用于在装备成功之后，取消技能勾选状态
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeRessignedSignature, const FGameplayTag&, AbilityTag);

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();	// 选中技能的AbilityTag
	FGameplayTag StatusTag = FGameplayTag();		// 选中技能的状态
	FGameplayTag TypeTag = FGameplayTag();	// 技能类型

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

	// 蓝图调用
	UFUNCTION(BlueprintCallable, Category = "SpellMenu")
	void HandleSpellPointAndEquipButton(const FGameplayTag& InAbilityTag);

	// 在技能菜单销毁时调用--清空现在选择的Ability
	UFUNCTION(BlueprintCallable, Category = "SpellMenu")
	void ClearSelectedAbility();

	// 花费技能点按钮按下事件
	UFUNCTION(BlueprintCallable, Category = "SpellMenu")
	void OnSpellPointButtonPressed();

	UFUNCTION(BlueprintCallable, Category = "SpellMenu")
	void OnEquipButtonPressed();

	// 技能装备槽被点击
	UFUNCTION(BlueprintCallable, Category = "SpellMenu")
	void OnSpellRowGlobePressed(const FGameplayTag& SlotInputTag, const FGameplayTag& SlotAbilityTypeTag);

	void EquipAbilityCallback(const FGameplayTag& AbilityTag, const FGameplayTag& NewStatusTag, const FGameplayTag& InputSlot, const FGameplayTag& PreInputSlot, bool bIsAbilitySwap);


private:
	// 技能点或者状态改变时，处理
	void HandleButtonEnableOnChanged(int32 NewSpellPoint, const FGameplayTag& NewStatusTag);

	// 处理点击已经选择的技能--取消选择
	UFUNCTION(BlueprintCallable)
	void GlobeSelfDeselect();

public:

	UPROPERTY(BlueprintAssignable, Category = "Ability|Spell")
	FSpellPointChangedSignature OnSpellPointChangedDelegate;

	// 控制两个按钮的可用与否
	UPROPERTY(BlueprintAssignable, Category = "SpellMenu")
	FSpellMenuButonSignature OnSpellMenuButtonDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SpellMenu|Equip")
	FWaitforEquipSelectSignature WaitforEquipSelectDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SpellMenu|Equip")
	FWaitforEquipSelectSignature StopEquipSelectDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SpellMenu|Equip")
	FSpellGlobeRessignedSignature SpellGlobeRessignedDelegate;

private:
	FSelectedAbility SelectedAbility;
	
	int32 CurrentSpellPoint = 0;

	bool bWaitforEquipSelect = false;

	// 当前选中技能的InputTag
	FGameplayTag InutTagOnSelectedAbility = FGameplayTag();
};
