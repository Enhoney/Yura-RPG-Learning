// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveMapDataInterface.h"
#include "Interaction/HighlightInterface.h"
#include "YuraRPG.h"
#include "CheckPoint.generated.h"

class USphereComponent;
class UMaterialInterface;
class UStaticMeshComponent;
class USceneComponent;

/**
 * 
 */
UCLASS()
class YURARPG_API ACheckPoint : public APlayerStart, public ISaveMapDataInterface , public IHighlightInterface
{
	GENERATED_BODY()

public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	/** SavedMapData Interface start*/
	virtual bool ShouldLoadingTransform_Implementation() const override;

	virtual void LoadActor_Implementation() override;
	/** SavedMapData Interface end*/

	/** HighlightInterface Interface start*/
	virtual void HighlightActor_Implementation() override;
	virtual void UnhighlightActor_Implementation() override;
	virtual void SetMoveToDestination_Implementation(FVector& OutDestination) override;

	/** HighlightInterface Interface end*/
protected:

	virtual void BeginPlay() override;

	// 重叠回调--执行存档操作
	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 使用定时器，让它逐渐变亮
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* GlowMaterialInstance, AActor* Player);


	// 创建动态材质，调用BlueprintImplementEvent函数
	void HandleGlowEffect(AActor* Player);

	// 在变亮完成之后调用，执行存档，回设材质，启用碰撞
	UFUNCTION(BlueprintCallable)
	virtual void FinishCheckpoint(AActor* Player);

public:
	// 是否激活
	UPROPERTY(SaveGame)
	bool bReached = false;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> CheckpointSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DestionationPoint;

	// 发光材质使用的--ValueName
	UPROPERTY(EditDefaultsOnly)
	FName MatParamName = FName("Glow");
	UPROPERTY(EditDefaultsOnly)
	float MatGlowValue = 50.f;

	// 高亮参数
	UPROPERTY(EditDefaultsOnly)
	int32 HighlightValue = CUSTOM_DEPTH_FOR_HIGHLIGHT_TAN;

	
};
