// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/YuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
struct FYuraAbilityInfo;
class UYuraUserWidget;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExpPercentChangedSignature, float, NewPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelChangedSignature, int32, NewLevel, bool , bIsLoading);


// 数据表行结构
USTRUCT(BlueprintType)
struct FUIWidgeRow : public FTableRowBase
{
	GENERATED_BODY()

	FUIWidgeRow(){}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UYuraUserWidget> MessageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* MessageImage = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgeRow, Message);



/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class YURARPG_API UOverlayWidgetController : public UYuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependiencies() override;

	template<typename T>
	T* GetDataTableRowByTag(const FGameplayTag& InTag, UDataTable* DataTable);


protected:

	void OnPlayerLevelChanged(int32 NewLevel, bool bIsLoading);

	void OnExpChanged(int32 NewExp);

	// 装备技能变更时，用于更新主菜单
	void EquipAbilityCallback(const FGameplayTag& AbilityTag, const FGameplayTag& NewStatusTag, const FGameplayTag& InputSlot, const FGameplayTag& PreInputSlot, bool bIsAbilitySwap);

public:
	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|GE Asset Massage")
	FMessageWidgetRowSignature OnMessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "State|Leveling Up")
	FOnExpPercentChangedSignature OnExpChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "State|Leveling Up")
	FOnLevelChangedSignature OnPlayerLevelChangedDelegate;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageTable;

};

template<typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(const FGameplayTag& InTag, UDataTable* DataTable)
{
	return DataTable->FindRow<T>(InTag.GetTagName(), TEXT(""));
}
