// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Actor/YuraProjectile.h"
#include "YuraFireBall.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraFireBall : public AYuraProjectile
{
	GENERATED_BODY()

protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
