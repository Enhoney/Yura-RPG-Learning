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

	// 仅对Pawn响应Overlap，响应重叠事件
	CheckpointSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CheckpointSphere"));
	CheckpointSphere->SetupAttachment(CheckpointMesh);
	CheckpointSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CheckpointSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CheckpointSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CheckpointSphere->SetGenerateOverlapEvents(true);
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


	// 保存原来的材质
	OriginCheckpointMeshMat = CheckpointMesh->GetMaterial(0);
	// 创建动态材质
	UMaterialInstanceDynamic* GlowMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(OriginCheckpointMeshMat, this);

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
		IPlayerInterface::Execute_SaveProgress(Player, PlayerStartTag);
	}

}
