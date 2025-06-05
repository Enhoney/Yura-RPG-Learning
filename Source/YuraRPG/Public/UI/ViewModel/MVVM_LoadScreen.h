// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.h"
#include "MVVM_LoadScreen.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitViewModelForLoadSlot();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelFromIndex(int32 Index) const;

	/** EnterName Slot 按钮点击回调*/
	UFUNCTION(BlueprintCallable)
	void OnNewSlotButtonClicked(int32 SlotIndex, const FString& InPlayerName);

	/** NewGame Slot 按钮点击回调*/
	UFUNCTION(BlueprintCallable)
	void OnNewGameButtonClicked(int32 SlotIndex);

	/** Select Slot 按钮点击回调*/
	UFUNCTION(BlueprintCallable)
	void OnSlectSlotButtonClicked(int32 SlotIndex);

	/** Field Notify*/
	// Getter
	int32 GetLoadSlotNum() const { return LoadSlotNum; }

	// Setter
	void SetLoadSlotNum(int32 InLoadSlotNum);


private:
	// 存储
	UPROPERTY()
	TMap<int32 /* Index*/, UMVVM_LoadSlot*> LoadSlots;

	// 放置被GC回收
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;


private:
	// Field Notify
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 LoadSlotNum;
	
};
