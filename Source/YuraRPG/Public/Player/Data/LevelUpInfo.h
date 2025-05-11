// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FYuraLevelUpInfo
{
	GENERATED_BODY()

	// 升到下一个等级所需经验
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Level")
	int32 ExpRequirement = 0;

	// 奖励的属性点
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Level")
	int32 AttributePointAward = 1;

	// 奖励的技能点
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Level")
	int32 SpellPointAward = 0;
};

/**
 * 
 */
UCLASS()
class YURARPG_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	// 我们约定，Index就代表等级，最低为1级，其中第一个做占位用，代表0级
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Level")
	TArray<FYuraLevelUpInfo> LevelUpInformation;

	UFUNCTION(BlueprintCallable)
	int32 FindCurrentLevelByExp(int32 CurExp) const;
};
