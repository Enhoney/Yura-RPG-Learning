// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxMana();
	// BlueprintNativeEvent，C++和蓝图都可以实现
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	// 捕获的属性
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
