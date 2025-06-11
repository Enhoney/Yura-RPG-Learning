// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LootTiers")
	TSubclassOf<AActor> LootClass;

	UPROPERTY(EditDefaultsOnly, Category = "LootTiers")
	float ChanceToSpawn = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "LootTiers")
	int32 MaxNumToSpawn = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LootTiers")
	bool bLootLevelOverride = true;
};

/**
 * 
 */
UCLASS(BlueprintType)
class YURARPG_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItensToSpawn() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "LootTiers")
	TArray<FLootItem> LootTiers;
	
};
