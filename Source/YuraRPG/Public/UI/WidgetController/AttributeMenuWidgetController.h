// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/YuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FYuraAttributeInfo;
struct FGameplayAttribute;
struct FGameplayTag;
class UYuraAttributeSet;
class UAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FYuraAttributeInfo&, AttributeInfo);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class YURARPG_API UAttributeMenuWidgetController : public UYuraWidgetController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependiencies() override;

private:
	void BrodacastAttributeInfo(const FGameplayTag& TagToFind, const FGameplayAttribute& Attribute);

public:
	UPROPERTY(BlueprintAssignable,Category = "GAS|Attribute")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	// 数据资产
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Attribute")
	TObjectPtr<UAttributeInfo> AttributeInfoData;
	
};
