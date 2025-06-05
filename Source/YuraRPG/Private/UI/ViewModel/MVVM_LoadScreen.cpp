// Copyright Muci Yun.


#include "UI/ViewModel/MVVM_LoadScreen.h"

void UMVVM_LoadScreen::InitViewModelForLoadSlot()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelFromIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::OnNewSlotButtonClicked(int32 SlotIndex, const FString& InPlayerName)
{

}

void UMVVM_LoadScreen::OnNewGameButtonClicked(int32 SlotIndex)
{
	// 切换到EnterNameSlot
	LoadSlots[SlotIndex]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::OnSlectSlotButtonClicked(int32 SlotIndex)
{

}

void UMVVM_LoadScreen::SetLoadSlotNum(int32 InLoadSlotNum)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotNum, InLoadSlotNum);
}
