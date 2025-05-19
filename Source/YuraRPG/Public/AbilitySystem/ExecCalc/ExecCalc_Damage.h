// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();

	// 这是一个BlueprintNative函数，所以在C++中提供实现的时候，必须加后缀
	void Execute_Implementation (const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	void DeterminingDebuff(const FGameplayEffectSpec& Spec, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters& EvalutionParameters) const;

private:
	bool CalculCriticalHitDamage(float& BaseDamage, const float SourceHitCriticalChance, const float TargetHitCriticalRes, const float SourceHitCriticalDamage) const;
	
};
