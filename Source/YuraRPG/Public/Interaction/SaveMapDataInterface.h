// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveMapDataInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveMapDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class YURARPG_API ISaveMapDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 是否要存储Transform
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ShouldLoadingTransform() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LoadActor();
};
