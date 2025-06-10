// Copyright Muci Yun.


#include "Game/MapEntrance.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

void AMapEntrance::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("YuraCharacter.Player")))
	{
		HandleGlowEffect(OtherActor);
	}
}

void AMapEntrance::FinishCheckpoint(AActor* Player)
{
	// 存档
	if (Player->Implements<UPlayerInterface>())
	{
		FString DestinationMapName = DestinationMap.ToSoftObjectPath().GetAssetName();;
		IPlayerInterface::Execute_SaveProgress(Player, DestinationPlayerStart, DestinationMapName);
	}

	// 前往目标地图
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
}
