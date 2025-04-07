// Copyright Muci Yun.


#include "YuraCharacterBase.h"

// Sets default values
AYuraCharacterBase::AYuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AYuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AYuraCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AYuraCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

