// Copyright Muci Yun.


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	// 可以将枚举写为TEnumAsByte，这样就可以直接通过GetValue来获取数值了
	const int32 SlotStatusToNum = static_cast<int32>(SlotStatus);
	SetWidgetSwitcherIndex.Broadcast(SlotStatusToNum);	// 仅做测试使用
}

void UMVVM_LoadSlot::SetLoadSlotName(const FString& InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

void UMVVM_LoadSlot::SetPlayerName(const FString& InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UMVVM_LoadSlot::SetMapName(const FString& InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

void UMVVM_LoadSlot::SetPlayerLevel(int32 InPlayerLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, InPlayerLevel);
}
