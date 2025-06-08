// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

// 切换SwitcherWidget
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, SwitcherIndex);

// SelectButton是否可用
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectButtonEnable, bool, bSelectButtonEnable);

/**
 * 
 */
UCLASS()
class YURARPG_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	// 这个用于在开始进入游戏的时候，确定这个槽是否有存档，如果有存档，广播的Index会是2
	void InitializeSlot();

	/** For Field Notify*/
	// Getter
	FString GetLoadSlotName() const { return LoadSlotName; }
	FString GetPlayerName() const { return PlayerName; }
	FString GetMapName() const { return MapName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }

	// Setter
	void SetLoadSlotName(const FString& InLoadSlotName);
	void SetPlayerName(const FString& InPlayerName);
	void SetMapName(const FString& InMapName);
	void SetPlayerLevel(int32 InPlayerLevel);

public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	UPROPERTY(BlueprintAssignable)
	FSelectButtonEnable SelectButtonEnable;

	UPROPERTY()
	ESaveSlotStatus SlotStatus;

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	FName PlayerStartTag;

private:
	// Field Notify
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 PlayerLevel;
	
};
