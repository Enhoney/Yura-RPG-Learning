// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/YuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
struct FYuraAbilityInfo;
class UYuraUserWidget;

class UAbilityInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);


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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoGivenSignature, const FYuraAbilityInfo&, YuraAbilityInfo);

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

	// 初始化赋予能力的回调
	UFUNCTION(BlueprintCallable)
	void OnStartupAbilitiesGiven();


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

	UPROPERTY(BlueprintAssignable, Category = "GAS|Ability Info")
	FAbilityInfoGivenSignature OnAbilityInfoGivenDelegate;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInformations;

};

template<typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(const FGameplayTag& InTag, UDataTable* DataTable)
{
	return DataTable->FindRow<T>(InTag.GetTagName(), TEXT(""));
}
