// Copyright Muci Yun.


#include "Actor/SpawnEnemy/EnemySpawnPoint.h"
#include "Character/YuraEnemy.h"

void AEnemySpawnPoint::SpawnEnemy()
{
	AYuraEnemy* EnemyToSpawn = GetWorld()->SpawnActorDeferred<AYuraEnemy>(EnemyClass, GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	EnemyToSpawn->SetCharacterLevel(EnemyLevel);

	EnemyToSpawn->FinishSpawning(GetActorTransform());
}
