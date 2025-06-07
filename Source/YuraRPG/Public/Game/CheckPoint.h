// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "CheckPoint.generated.h"

class USphereComponent;
class UMaterialInterface;
class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class YURARPG_API ACheckPoint : public APlayerStart
{
	GENERATED_BODY()

public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void BeginPlay() override;

	// 重叠回调--执行存档操作
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 使用定时器，让它逐渐变亮
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* GlowMaterialInstance, AActor* Player);


	// 创建动态材质，调用BlueprintImplementEvent函数
	void HandleGlowEffect(AActor* Player);

	// 在变亮完成之后调用，执行存档，回设材质，启用碰撞
	UFUNCTION(BlueprintCallable)
	void FinishCheckpoint(AActor* Player);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> OriginCheckpointMeshMat;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CheckpointSphere;

	
};
