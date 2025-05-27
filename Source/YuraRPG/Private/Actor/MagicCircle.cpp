// Copyright Muci Yun.


#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	MaginCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("MaginCircleDecal"));
	MaginCircleDecal->SetupAttachment(GetRootComponent());

}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicCircle::SetMagicCircleMaterial(UMaterialInterface* InMagicCircleMat)
{
	MaginCircleDecal->SetDecalMaterial(InMagicCircleMat);
}

