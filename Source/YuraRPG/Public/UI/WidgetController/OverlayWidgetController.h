// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/YuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;

class UYuraUserWidget;

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

protected:
	/** 属性变更回调*/
	void BroadcastHealthChanged(const FOnAttributeChangeData& Data) const;

	void BroadcasMaxHealthChanged(const FOnAttributeChangeData& Data) const;

	void BroadcastManaChanged(const FOnAttributeChangeData& Data) const;

	void BroadcastMaxManaChanged(const FOnAttributeChangeData& Data) const;

public:
	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageTable;

	template<typename T>
	T* GetDataTableRowByTag(const FGameplayTag& InTag, UDataTable* DataTable);
	
	
};

template<typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(const FGameplayTag& InTag, UDataTable* DataTable)
{
	return DataTable->FindRow<T>(InTag.GetTagName(), TEXT(""));
}
