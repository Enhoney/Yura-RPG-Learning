// Copyright Muci Yun.


#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Game/YuraGameModeBase.h"
#include "Game/YuraGameInstance.h"

void UMVVM_LoadScreen::InitViewModelForLoadSlot()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(TEXT("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(TEXT("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(TEXT("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelFromIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::LoadSaveData()
{
	if (AYuraGameModeBase* GameMode = Cast<AYuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		for (const TTuple<int32, UMVVM_LoadSlot*>& TempLoadSlot : LoadSlots)
		{
			ULoadScreenSaveGame* SaveObject = GameMode->GetSaveSlotData(TempLoadSlot.Value->GetLoadSlotName(), TempLoadSlot.Key);
			TempLoadSlot.Value->SetPlayerName(SaveObject->PlayerName);
			TempLoadSlot.Value->SlotStatus = SaveObject->LoadSlotStatus;
			TempLoadSlot.Value->SetMapName(SaveObject->MapName);
			TempLoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
			// 广播初始加载的数据
			TempLoadSlot.Value->InitializeSlot();
		}
	}
	
}

void UMVVM_LoadScreen::OnNewSlotButtonClicked(int32 SlotIndex, const FString& InPlayerName)
{
	// 创建存档
	if (AYuraGameModeBase* GameMode = Cast<AYuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		LoadSlots[SlotIndex]->SetPlayerName(InPlayerName);
		LoadSlots[SlotIndex]->SlotStatus = ESaveSlotStatus::Taken;
		
		// 新档，使用默认地图
		LoadSlots[SlotIndex]->SetMapName(GameMode->DefaultMapName);
		// 新档。使用默认的PlayerStart
		LoadSlots[SlotIndex]->PlayerStartTag = GameMode->DefaultPlayerStartTag;
		// 保存到存档文件中
		GameMode->SaveSlotData(LoadSlots[SlotIndex], SlotIndex);
		// 切换到Taken
		LoadSlots[SlotIndex]->InitializeSlot();
	}
	
}

void UMVVM_LoadScreen::OnNewGameButtonClicked(int32 SlotIndex)
{
	// 切换到EnterNameSlot
	LoadSlots[SlotIndex]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::OnSlectSlotButtonClicked(int32 SlotIndex)
{
	// 让其他Slot中的SelectButton可用
	for (TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key != SlotIndex)
		{
			LoadSlot.Value->SelectButtonEnable.Broadcast(true);
		}
		else
		{
			LoadSlot.Value->SelectButtonEnable.Broadcast(false);
		}
	}

	// 存储选中的Slot
	SelectedSlot = LoadSlots[SlotIndex];
	// 让下面的Play和Delete按钮可用，也需要代理来做
	SelectButtonSelectedOnTaken.Broadcast();
}

void UMVVM_LoadScreen::OnPlayButtonClicked()
{
	if (IsValid(SelectedSlot))
	{
		// 前往指定地图
		if (AYuraGameModeBase* GameMode = Cast<AYuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			// 设置PlayerStart
			if (UYuraGameInstance* YuraGameInstance = Cast<UYuraGameInstance>(GameMode->GetGameInstance()))
			{
				YuraGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
				YuraGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
				YuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
			}
			GameMode->TravelToMap(SelectedSlot);
		}
	}
}

void UMVVM_LoadScreen::OnDeleteSlotButtonClicked()
{
	if (IsValid(SelectedSlot))
	{
		// 删除存档
		if (AYuraGameModeBase* GameMode = Cast<AYuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			GameMode->DeleteSaveSlotDate(SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex);
		}

		// 刷新UI
		SelectedSlot->SlotStatus = ESaveSlotStatus::Vacant;
		SelectedSlot->SetPlayerName(FString());
		SelectedSlot->InitializeSlot();
		SelectedSlot->SelectButtonEnable.Broadcast(true);
	}
}

void UMVVM_LoadScreen::SetLoadSlotNum(int32 InLoadSlotNum)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotNum, InLoadSlotNum);
}
