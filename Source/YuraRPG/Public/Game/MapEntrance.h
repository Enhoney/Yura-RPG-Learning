// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Game/CheckPoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()

public:
	

protected:
	// 重叠回调--执行存档操作
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// 在变亮完成之后调用，执行存档，回设材质，启用碰撞
	virtual void FinishCheckpoint(AActor* Player) override;

public:
	// 目标地图
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	// 目标位置
	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStart = FName("DefaultPlayerStart");
};
