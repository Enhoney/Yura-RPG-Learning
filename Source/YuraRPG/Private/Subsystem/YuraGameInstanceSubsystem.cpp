// Copyright Muci Yun.


#include "Subsystem/YuraGameInstanceSubsystem.h"
#include "Game/YuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Interaction/SaveMapDataInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void UYuraGameInstanceSubsystem::SaveMapData(UWorld* World, ULoadScreenSaveGame* SaveGame)
{
	if (!IsValid(World)) return;

	if (SaveGame)
	{
		// 获取MapName
		// 直接获取的，里面会有一些其他信息--StreamingLevelsPrefix，我们可以手动删掉，这样就是真正的MapName了
		FString MapName = World->GetMapName();
		MapName.RemoveFromStart(World->StreamingLevelsPrefix);

		if (!SaveGame->HasMapData(MapName))
		{
			// 如果没有，就添加一个进去
			FSavedMapData MapDataToSave;
			MapDataToSave.MapAssetName = MapName;
			SaveGame->SavedMapData.AddUnique(MapDataToSave);
		}

		// 这个时候，肯定就是有了的
		FSavedMapData& MapDataSaved = SaveGame->GetSavedMapDataFromName_Ref(MapName);
		// 清理掉原来的数据
		MapDataSaved.SavedActors.Empty();

		// 寻找地图中所有需要保存的Actor
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (!IsValid(Actor) || !Actor->Implements<USaveMapDataInterface>()) continue;

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			if (ISaveMapDataInterface::Execute_ShouldLoadingTransform(Actor))
			{
				SavedActor.ActorTransform = Actor->GetActorTransform();
			}

			// 序列化
			FMemoryWriter MemoryWriter(SavedActor.Bytes);

			FObjectAndNameAsStringProxyArchive Archve(MemoryWriter, true);
			Archve.ArIsSaveGame = true;
			// Actor的序列化
			Actor->Serialize(Archve);

			// 添加到存档
			MapDataSaved.SavedActors.AddUnique(SavedActor);
		}

	}
}

void UYuraGameInstanceSubsystem::LoadMapData(UWorld* World, const ULoadScreenSaveGame* SaveGame)
{
	if (!IsValid(World)) return;

	// 获取MapName
	FString MapName = World->GetMapName();
	MapName.RemoveFromStart(World->StreamingLevelsPrefix);
	// 查找MapData
	FSavedMapData MapDataSaved = SaveGame->GetSavedMapDataFromName(MapName);

	// 寻找地图中所有需要保存的Actor
	for (FActorIterator It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<USaveMapDataInterface>()) continue;

		for (FSavedActor SavedAcor : MapDataSaved.SavedActors)
		{
			if (SavedAcor.ActorName == Actor->GetFName())
			{
				if (ISaveMapDataInterface::Execute_ShouldLoadingTransform(Actor))
				{
					Actor->SetActorTransform(SavedAcor.ActorTransform);
				}

				// 反序列化存储的Actor成员
				FMemoryReader MemoryReader(SavedAcor.Bytes);

				FObjectAndNameAsStringProxyArchive Archve(MemoryReader, true);
				Archve.ArIsSaveGame = true;
				// Actor的反序列化
				Actor->Serialize(Archve);

				// 使读取的参数生效
				ISaveMapDataInterface::Execute_LoadActor(Actor);
			}
			
		}
	}
}
