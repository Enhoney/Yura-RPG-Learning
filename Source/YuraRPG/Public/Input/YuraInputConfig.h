// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "YuraInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FYuraInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* FindInputActionByTag(const FGameplayTag& InTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<FYuraInputAction> AbilityInputActions;
	
};
