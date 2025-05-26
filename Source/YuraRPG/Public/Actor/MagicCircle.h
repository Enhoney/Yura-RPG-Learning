// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

class UDecalComponent;

UCLASS()
class YURARPG_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicCircle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetMagicCircleMaterial(UMaterialInterface* InMagicCircleMat);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MaginCircleDecal;

};
