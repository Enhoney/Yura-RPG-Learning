// Copyright Muci Yun.


#include "AbilitySystem/ModMagCal/MMC_MaxHealth.h"

#include "AttributeSets/YuraAttributeSet.h"

#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// 获取Vigor
	VigorDef.AttributeToCapture = UYuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// 不使用SnapShot
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 从源和目标收集标签，因为这会影响哪些buff应该使用
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 需要通过这个东西才能够拿到属性
	FAggregatorEvaluateParameters EvalutionParameters;
	EvalutionParameters.SourceTags = SourceTags;
	EvalutionParameters.TargetTags = TargetTags;

	float Vigor = 0.0f;
	// 获取属性的之并保存在Vigor中，这拿到的是目标的，因为在这里我们设置了--VigorDef
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvalutionParameters, Vigor);
	Vigor = FMath::Max(0.0f, Vigor);

	// 获取等级,Spec.GetContext().GetSourceObject()--创建这个GE的对象，最终就会拿到玩家或者敌人的Character
	// 因为这个GE只会对自己施加，所以就是SourceObject，那么在这里能不能拿到TargetActor呢？
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	check(CombatInterface);
	const int32 CharacterLevel = CombatInterface->GetCharacterLevel();

	// 我们约定等级上限为100，那么血量范围就是120~2000
	return 10.f * Vigor + 20.f * CharacterLevel;
}
