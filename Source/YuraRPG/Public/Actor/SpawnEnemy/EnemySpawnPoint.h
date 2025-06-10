// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "EnemySpawnPoint.generated.h"

class AYuraEnemy;

/**
 * 
 */
UCLASS()
class YURARPG_API AEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EnemySpawn")
	void SpawnEnemy();

private:
	UPROPERTY(EditAnywhere, Category = "EnemySpawn")
	TSubclassOf<AYuraEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "EnemySpawn")
	int32 EnemyLevel;

	
};
