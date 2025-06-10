// Copyright Muci Yun.


#include "Actor/SpawnEnemy/EnemySpawnVolume.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemySpawnVolume::AEnemySpawnVolume()
{

	PrimaryActorTick.bCanEverTick = false;

	// 这个东西应该只在服务器上存在，只在服务器上刷怪
	bReplicates = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	TriggerBox->SetupAttachment(GetRootComponent());
	
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetCollisionObjectType(ECC_WorldStatic);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
}

bool AEnemySpawnVolume::ShouldLoadingTransform_Implementation() const
{
	return false;
}

void AEnemySpawnVolume::LoadActor_Implementation()
{
	if (bActivated)
	{
		// 一次性刷完怪直接销毁
		Destroy();
	}
}

// Called when the game starts or when spawned
void AEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawnVolume::OnTriggerBoxBeginOverlap);
	
}

void AEnemySpawnVolume::OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("YuraCharacter.Player")))
	{
		bActivated = true;
		TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 刷怪
		if (!EnemySpawnPoints.IsEmpty())
		{
			for (AEnemySpawnPoint* EnemySpawnPoint : EnemySpawnPoints)
			{
				if (IsValid(EnemySpawnPoint))
				{
					EnemySpawnPoint->SpawnEnemy();
				}
			}
		}

	}
}


