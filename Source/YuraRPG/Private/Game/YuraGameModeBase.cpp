// Copyright Muci Yun.


#include "YuraGameModeBase.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Kismet/GameplayStatics.h"
#include "LoadScreenSaveGame.h"

void AYuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	// 首先检查这个Slot对应的有没有存档，没有才创建，有的话就删除原来的存档，
	// 因为这个函数只会在EnterName的时候调用，一般来说这个时候不会有存档在这里的，除非逻辑有误
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}

	// 创建存档
	ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass));
	SaveGame->LoadSlotName = LoadSlot->GetLoadSlotName();
	SaveGame->LoadSlotIndex = SlotIndex;
	SaveGame->PlayerName = LoadSlot->GetPlayerName();
	SaveGame->LoadSlotStatus = LoadSlot->SlotStatus;
	// 新存档使用默认地图
	SaveGame->MapName = DefaultMapName;

	// 保存--他会保存到本地磁盘中的
	UGameplayStatics::SaveGameToSlot(SaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);

}

ULoadScreenSaveGame* AYuraGameModeBase::GetSaveSlotData(const FString& InSlotName, int32 SlotIndex) const
{
	ULoadScreenSaveGame* OutSaveGameData = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(InSlotName, SlotIndex))
	{
		OutSaveGameData =  Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(InSlotName, SlotIndex));
	}
	else
	{
		OutSaveGameData = Cast<ULoadScreenSaveGame>(UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass));
	}
	
	return OutSaveGameData;
}

void AYuraGameModeBase::DeleteSaveSlotDate(const FString& InSlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(InSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(InSlotName, SlotIndex);
	}
}

void AYuraGameModeBase::TravelToMap(UMVVM_LoadSlot* LoadSlot)
{
	TSoftObjectPtr<UWorld> MapToTravel = NameToMaps.FindChecked(LoadSlot->GetMapName());

	// 打开关卡
	// 这个函数其实调用的也是OpenLevel，内部实现是这样的：
	// const FName LevelName = FName(*FPackageName::ObjectPathToPackageName(Level.ToString()));
	// UGameplayStatics::OpenLevel(WorldContextObject, LevelName, bAbsolute, Options);
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, MapToTravel);
}

void AYuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	NameToMaps.Add(DefaultMapName, DefaultMap);
}
