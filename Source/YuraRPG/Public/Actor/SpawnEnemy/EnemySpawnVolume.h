// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveMapDataInterface.h"
#include "Actor/SpawnEnemy/EnemySpawnPoint.h"
#include "EnemySpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class YURARPG_API AEnemySpawnVolume : public AActor, public ISaveMapDataInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnVolume();

	/** SaveMapDataInterface Start*/
	virtual bool ShouldLoadingTransform_Implementation() const override;

	virtual void LoadActor_Implementation() override;
	/** SaveMapDataInterface End*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(SaveGame)
	bool bActivated = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> TriggerBox;

	// 存储刷怪点--我们希望在实例中编辑
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AEnemySpawnPoint>> EnemySpawnPoints;

};
