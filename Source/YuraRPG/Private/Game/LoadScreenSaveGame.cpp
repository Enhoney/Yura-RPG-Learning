// Copyright Muci Yun.


#include "Game/LoadScreenSaveGame.h"

FSavedMapData ULoadScreenSaveGame::GetSavedMapDataFromName(const FString& MapAssetName) const
{
	for (FSavedMapData MapData : SavedMapData)
	{
		if (MapData.MapAssetName == MapAssetName)
		{
			return MapData;
		}
	}
	

	return FSavedMapData();
}

FSavedMapData& ULoadScreenSaveGame::GetSavedMapDataFromName_Ref(const FString& MapAssetName)
{
	int32 MapIndex = 0;
	for (FSavedMapData& MapData : SavedMapData)
	{
		if (MapData.MapAssetName == MapAssetName)
		{
			break;
		}
		++MapIndex;
	}


	return SavedMapData[MapIndex];
}

bool ULoadScreenSaveGame::HasMapData(const FString& MapAssetName) const
{
	for (FSavedMapData MapData : SavedMapData)
	{
		if (MapData.MapAssetName == MapAssetName)
		{
			return true;
		}
	}

	return false;
}
