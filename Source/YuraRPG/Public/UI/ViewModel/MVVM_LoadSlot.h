// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, SwitcherIndex);

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

	// Setter
	void SetLoadSlotName(const FString& InLoadSlotName);

public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

private:
	// Field Notify
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;

	
};
