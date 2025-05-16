// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityInfo.generated.h"


USTRUCT(BlueprintType)
struct FYuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|AbilityInfo")
	FGameplayTag AbilityTag = FGameplayTag();

	// 技能类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|AbilityInfo")
	FGameplayTag AbilityTypeTag = FGameplayTag();
	
	UPROPERTY(BlueprintReadOnly, Category = "UI|AbilityInfo")
	FGameplayTag AbilityInputTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly, Category = "UI|AbilityInfo")
	FGameplayTag AbilityStatusTag = FGameplayTag();

	// 用于发起GameplayEvent，显示冷却时间的
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|AbilityInfo")
	FGameplayTag AbilityCooldownTag = FGameplayTag();

	// 技能图标
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|AbilityInfo")
	TObjectPtr<const UTexture2D> Icon = nullptr;

	// 技能图标背景
	// 这种写法相当于是const UMaterialInterface*，表示数据不可修改，指向可以修改
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|AbilityInfo")
	TObjectPtr<const UMaterialInterface> GlobeMat = nullptr;

	// 技能解锁需要的等级
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|AbilityInfo")
	int32 LevelRequirement = 1;

	// 对应的技能类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TSubclassOf<UGameplayAbility> AbilityClass;

};

/**
 * 
 */
UCLASS()
class YURARPG_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YuraAbilityInformation")
	TArray<FYuraAbilityInfo> YuraAbilityInfos;

	UFUNCTION(BlueprintPure, Category = "YuraAbilityInformation")
	FYuraAbilityInfo FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bWarningIfNotFound = false) const;
	
};
