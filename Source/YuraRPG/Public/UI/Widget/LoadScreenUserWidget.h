// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadScreenUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API ULoadScreenUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintInitializeWidget();
	
};
