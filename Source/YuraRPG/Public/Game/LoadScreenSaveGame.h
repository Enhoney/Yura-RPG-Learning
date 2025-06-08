// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(BlueprintType)
enum class ESaveSlotStatus : uint8
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT(BlueprintType)
struct FSavedAbilityInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveGame")
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveGame")
	FGameplayTag AbilityTag = FGameplayTag();

	// 状态--是否解锁
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveGame")
	FGameplayTag AbilityStatusTag = FGameplayTag();

	// 输入
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveGame")
	FGameplayTag AbilityInputTag = FGameplayTag();

	// 主动还是被动
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveGame")
	FGameplayTag AbilityTypeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveGame")
	int32 AbilityLevel = 1;
};

USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform ActorTransform;

	// 字节码，用于保存那些可以序列化的成员变量
	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& SavedAcytor1, const FSavedActor& SavedAcytor2)
{
	return SavedAcytor1.ActorName == SavedAcytor2.ActorName;
}

USTRUCT()
struct FSavedMapData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString MapAssetName;

	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

inline bool operator==(const FSavedMapData& SavedMap1, const FSavedMapData& SavedMap2)
{
	return SavedMap1.MapAssetName == SavedMap2.MapAssetName;
}

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

	// 是否已保存关键数据
	UPROPERTY()
	bool bVitalDataSaved = false;

	/** Player*/

	UPROPERTY()
	int32 Level = 1;

	UPROPERTY()
	int32 Exp = 0;

	UPROPERTY()
	int32 AttributePoint = 5;

	UPROPERTY()
	int32 SpellPoint = 3;

	/** Attribute*/

	UPROPERTY()
	float Strength = 0.f;

	UPROPERTY()
	float Intelligence = 0.f;

	UPROPERTY()
	float Resilience = 0.f;

	UPROPERTY()
	float Vigor = 0.f;

	// Ability
	UPROPERTY()
	TArray<FSavedAbilityInfo> SavedAbilities;

	// Saved Map Data
	UPROPERTY()
	TArray<FSavedMapData> SavedMapData;

public:
	FSavedMapData GetSavedMapDataFromName(const FString& MapAssetName) const;

	FSavedMapData& GetSavedMapDataFromName_Ref(const FString& MapAssetName);

	bool HasMapData(const FString& MapAssetName) const;

};
