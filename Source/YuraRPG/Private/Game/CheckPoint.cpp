// Copyright Muci Yun.


#include "Game/CheckPoint.h"
#include "Components/SphereComponent.h"
#include "Interaction/PlayerInterface.h"


ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	PrimaryActorTick.bCanEverTick = false;

	// 阻挡所有，不响应重叠事件
	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheckpointMesh"));
	CheckpointMesh->SetupAttachment(GetRootComponent());
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	CheckpointMesh->SetGenerateOverlapEvents(false);

	CheckpointMesh->SetCustomDepthStencilValue(HighlightValue);
	CheckpointMesh->MarkRenderStateDirty();

	// 仅对Pawn响应Overlap，响应重叠事件
	CheckpointSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CheckpointSphere"));
	CheckpointSphere->SetupAttachment(CheckpointMesh);
	CheckpointSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CheckpointSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CheckpointSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CheckpointSphere->SetGenerateOverlapEvents(true);

	DestionationPoint = CreateDefaultSubobject<USceneComponent>(TEXT("DestinationPoint"));
	DestionationPoint->SetupAttachment(CheckpointMesh);
}

bool ACheckPoint::ShouldLoadingTransform_Implementation() const
{
	return false;
}

void ACheckPoint::LoadActor_Implementation()
{
	if (bReached)
	{
		// 直接修改为无碰撞
		CheckpointSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 创建动态材质
		UMaterialInstanceDynamic* GlowMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
		// 发光
		GlowMaterialInstanceDynamic->SetScalarParameterValue(MatParamName, MatGlowValue);
		// 修改材质为动态材质
		CheckpointMesh->SetMaterial(0, GlowMaterialInstanceDynamic);
	}
}

void ACheckPoint::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

void ACheckPoint::UnhighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(false);
}

void ACheckPoint::SetMoveToDestination_Implementation(FVector& OutDestination)
{
	OutDestination = DestionationPoint->GetComponentLocation();
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	CheckpointSphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereBeginOverlap);
}

void ACheckPoint::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("YuraCharacter.Player")))
	{
		bReached = true;
		HandleGlowEffect(OtherActor);
	}
}

void ACheckPoint::HandleGlowEffect(AActor* Player)
{
	// 直接修改为无碰撞
	CheckpointSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 创建动态材质
	UMaterialInstanceDynamic* GlowMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);

	// 修改材质为动态材质
	CheckpointMesh->SetMaterial(0, GlowMaterialInstanceDynamic);

	// 开始发亮
	CheckpointReached(GlowMaterialInstanceDynamic, Player);
}

void ACheckPoint::FinishCheckpoint(AActor* Player)
{
	// 存档
	if (Player->Implements<UPlayerInterface>())
	{
		FString CurrentMapName = GetWorld()->GetMapName();
		CurrentMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		IPlayerInterface::Execute_SaveProgress(Player, PlayerStartTag, CurrentMapName);
	}

}
