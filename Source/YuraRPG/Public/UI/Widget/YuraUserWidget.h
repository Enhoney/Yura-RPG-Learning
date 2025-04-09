// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "YuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	

public:

	// WidgetController
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

};
