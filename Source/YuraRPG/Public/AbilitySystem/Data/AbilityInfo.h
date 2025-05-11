// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FYuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|AbilityInfo")
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly, Category = "UI|AbilityInfo")
	FGameplayTag AbilityInputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|AbilityInfo")
	FGameplayTag AbilityCooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|AbilityInfo")
	TObjectPtr<const UTexture2D> Icon = nullptr;

	// 这种写法相当于是const UMaterialInterface*，表示数据不可修改，指向可以修改
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|AbilityInfo")
	TObjectPtr<const UMaterialInterface> GlobeMat = nullptr;

};

/**
 * 
 */
UCLASS()
class YURARPG_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YuraAbilityInformation")
	TArray<FYuraAbilityInfo> YuraAbilityInfos;

	UFUNCTION(BlueprintPure, Category = "YuraAbilityInformation")
	FYuraAbilityInfo FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bWarningIfNotFound = false) const;
	
};
