// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

class UAbilityInfo;

USTRUCT(BlueprintType)
struct FWidgetControllerParam
{
	GENERATED_BODY()

public:
	FWidgetControllerParam(){}
	FWidgetControllerParam(APlayerController* InPC, APlayerState* InPS, UAbilitySystemComponent* InASC, UAttributeSet* InAS)
		: PlayerState(InPS), PlayerController(InPC), AbilitySystemComponent(InASC), AttributeSet(InAS)
	{

	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent =  nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet =  nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoGivenSignature, const FYuraAbilityInfo&, YuraAbilityInfo);

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraWidgetController : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetControllrtParams(const FWidgetControllerParam& InWCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	virtual void BindCallbacksToDependiencies();

	// 初始化赋予能力的回调
	UFUNCTION(BlueprintCallable)
	void BroadcastYuraAbilityInfo();

public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Ability Info")
	FAbilityInfoGivenSignature OnAbilityInfoGivenDelegate;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	// 存储Ability 信息
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInformations;
	
};
