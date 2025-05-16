// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/YuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

// 用于显示技能点
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellPointChangedSignature, int32, NewSpellPoints);

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

public:

	UPROPERTY(BlueprintAssignable, Category = "Ability|Spell")
	FSpellPointChangedSignature OnSpellPointChangedDelegate;
	
};
