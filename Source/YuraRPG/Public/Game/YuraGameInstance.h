// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "YuraGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex;
};
