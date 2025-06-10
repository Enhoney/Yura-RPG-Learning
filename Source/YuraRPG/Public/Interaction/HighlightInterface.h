// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighlightInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UHighlightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class YURARPG_API IHighlightInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 高亮于取消高亮
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HighlightActor();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnhighlightActor();

	// 设置目的地
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMoveToDestination(FVector& OutDestination);
};
