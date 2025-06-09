// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "YuraGameInstanceSubsystem.generated.h"

class ULoadScreenSaveGame;

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }



	// 保存MapData到存档
	void SaveMapData(UWorld* World, ULoadScreenSaveGame* SaveGame);

	// 读取MapData到关卡
	void LoadMapData(UWorld* World, const ULoadScreenSaveGame* SaveGame);
};
