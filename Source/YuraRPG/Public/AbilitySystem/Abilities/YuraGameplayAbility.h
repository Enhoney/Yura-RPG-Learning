// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "YuraGameplayAbility.generated.h"

struct FGameplayTag;
struct FTaggedMontage;

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetCurrentLevelDescription(int Level);

	virtual FString GetNextLevelDescription(int Level);

	static FString GetLockedDescription(int Level);

protected:
	// 设置WarpingTargetFacing
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetWarpingTargetFacingFromLocation(const FVector& TargetLocation);

	// 获取一个随机的攻击动画蒙太奇和对应的Tag
	UFUNCTION(BlueprintPure, Category = "Ability")
	FTaggedMontage GetRandTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages) const;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag StartUpInputTag;
	
};
