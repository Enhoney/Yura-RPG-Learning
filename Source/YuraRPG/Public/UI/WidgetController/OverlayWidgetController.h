// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/YuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

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
	
};
