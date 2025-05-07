// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/YuraGameplayAbility.h"
#include "YuraSummonGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraSummonGameplayAbility : public UYuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Summoning Ability")
	TArray<FVector> GetSummonSpawnLocations();

	// 随机找一个召唤物来放置
	UFUNCTION(BlueprintCallable, Category = "Summoning Ability")
	AActor* SpawnRandMinionAtLocation(const FVector& SpawnLocation);

	// 放置指定召唤物
	UFUNCTION(BlueprintCallable, Category = "Summoning Ability")
	AActor* SpawnPointMinionAtLocation(const TSubclassOf<APawn> PawnToSpawn, const FVector& SpawnLocation);

	// 获取随机一个召唤物类
	UFUNCTION(BlueprintPure, Category = "Summoning Ability")
	TSubclassOf<APawn> GetRandMinionClass() const;
protected:
	// 召唤的数量
	UPROPERTY(EditDefaultsOnly, Category = "Summoning Ability")
	int32 MinionsNum = 5;

	// 召唤的时候，会随机从中选取一种进行召唤
	UPROPERTY(EditDefaultsOnly, Category = "Summoning Ability")
	TArray<TSubclassOf<APawn>> MinionClasses;

	// 召唤时计算位置的范围--在召唤者周身一个环形或者扇形范围内，我们选择在身前的一个扇形范围内生成
	UPROPERTY(EditDefaultsOnly, Category = "Summoning Ability")
	float MinSpawnRadius = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Summoning Ability")
	float MaxSpawnRadius = 250;

	// 扇形的角度--默认身前90°
	UPROPERTY(EditDefaultsOnly, Category = "Summoning Ability")
	float SpawnSpread = 90;

	
};
