// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(BlueprintType)
enum class ESaveSlotStatus : uint8
{
	Vacant,
	EnterName,
	Taken
};

/**
 * 
 */
UCLASS()
class YURARPG_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// 存档相关
	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex;

	UPROPERTY()
	ESaveSlotStatus LoadSlotStatus = ESaveSlotStatus::Vacant;

	// 游戏内容相关

	UPROPERTY()
	FString PlayerName = FString();

	UPROPERTY()
	FString MapName = FString();

	// 相当于玩家的位置
	UPROPERTY()
	FName PlayerStartTag;

	/** Player*/

	UPROPERTY()
	int32 Level = 0;

	UPROPERTY()
	int32 Exp = 0;

	UPROPERTY()
	int32 AttributePoint = 0;

	UPROPERTY()
	int32 SpellPoint = 0;

	/** Attribute*/

	UPROPERTY()
	float Strength = 0.f;

	UPROPERTY()
	float Intelligence = 0.f;

	UPROPERTY()
	float Resilience = 0.f;

	UPROPERTY()
	float Vigor = 0.f;
};
