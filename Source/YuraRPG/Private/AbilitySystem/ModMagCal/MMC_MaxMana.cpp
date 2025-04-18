// Copyright Muci Yun.


#include "AbilitySystem/ModMagCal/MMC_MaxMana.h"

#include "Interaction/CombatInterface.h"
#include "AttributeSets/YuraAttributeSet.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UYuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvalutionParameters;
	EvalutionParameters.SourceTags = SourceTags;
	EvalutionParameters.TargetTags = TargetTags;

	float Intellignece = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvalutionParameters, Intellignece);
	Intellignece = FMath::Max(0.f, Intellignece);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	check(CombatInterface);
	const int32 CharacterLevel = CombatInterface->GetCharacterLevel();

	// 210 ~ 2500，等级成长系数较低
	return 20.f * Intellignece + 10 * CharacterLevel;
}
