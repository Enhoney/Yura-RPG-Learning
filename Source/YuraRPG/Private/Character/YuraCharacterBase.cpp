// Copyright Muci Yun.


#include "YuraCharacterBase.h"

AYuraCharacterBase::AYuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AYuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

