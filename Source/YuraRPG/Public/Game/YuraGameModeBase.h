// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "YuraGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
class UMVVM_LoadSlot;
class ULoadScreenSaveGame;

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// 存档创建的地方，从EnterName的NewSLot点击时触发，从MVVM_LoadSlot中获取输入的PlayerName以及Slot信息
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

	// 加载存档
	ULoadScreenSaveGame* GetSaveSlotData(const FString& InSlotName, int32 SlotIndex) const;

	// 删除存档
	void DeleteSaveSlotDate(const FString& InSlotName, int32 SlotIndex);

	void TravelToMap(UMVVM_LoadSlot* LoadSlot);

protected:
	virtual void BeginPlay() override;

public:
	// Enemy 初始化属性的资产
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Character Attribute Default")
	TObjectPtr<UCharacterClassInfo> DefaultEnemyInfo;

	// Ability Info
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Character Ability Info Default")
	TObjectPtr<UAbilityInfo> DefaultAbilityInfo;

	// 存档类
	UPROPERTY(EditDefaultsOnly, Category = "Load Screen Save Game")
	TSubclassOf<ULoadScreenSaveGame> LoadScreenSaveGameClass;



	// 初始地图
	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	// 映射地图及其名字
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> NameToMaps;
	
};
