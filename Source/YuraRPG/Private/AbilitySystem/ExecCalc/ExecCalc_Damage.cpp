// Copyright Muci Yun.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AttributeSets/YuraAttributeSet.h"

#include "YuraGameplayTags.h"

struct FYuraDamageStatics
{
	// 这个宏是GEEC中提供的
	// 这个FProperty就是帮助我们拿到Attribute的
	/** 
	 *	#define DECLARE_ATTRIBUTE_CAPTUREDEF(P) \
	 *  FProperty* P##Property; \
	 *  FGameplayEffectAttributeCaptureDefinition P##Def; \
	 */
	/** Target Attribute*/
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);

	/** Source Attribute*/
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);

	// 捕获Damage属性
	FYuraDamageStatics()
	{
		// 这个宏也是GEEC特供的
		/**
		 *#define DEFINE_ATTRIBUTE_CAPTUREDEF(S, P, T, B) \
		 *{ \
		 *P##Property = FindFieldChecked<FProperty>(S::StaticClass(), GET_MEMBER_NAME_CHECKED(S, P)); \
		 *P##Def = FGameplayEffectAttributeCaptureDefinition(P##Property, EGameplayEffectAttributeCaptureSource::T, B); \
		 *}
		 */
		// 这种用法需要属性是Public的，否则无法访问

		/** Target Attribute*/
		DEFINE_ATTRIBUTE_CAPTUREDEF(UYuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UYuraAttributeSet, BlockChance, Target, false);

		/** Source Attribute*/
		DEFINE_ATTRIBUTE_CAPTUREDEF(UYuraAttributeSet, ArmorPenetration, Source, false);

	}
};

// 静态变量，这样每次拿到的都是同一个，算是一种单例吧
static FYuraDamageStatics& YuraDamageStatics()
{
	static FYuraDamageStatics DStatics;

	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// 记录捕获的属性
	/** Target Attribute*/
	RelevantAttributesToCapture.Add(YuraDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(YuraDamageStatics().BlockChanceDef);

	/** Source Attribute*/
	RelevantAttributesToCapture.Add(YuraDamageStatics().ArmorPenetrationDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvalutionParameters;
	EvalutionParameters.SourceTags = SourceTags;
	EvalutionParameters.TargetTags = TargetTags;

	// 通过SetByCaller拿到技能基础伤害
	float BaseDamage = Spec.GetSetByCallerMagnitude(FYuraGameplayTags::Get().Damage);

	// 获取目标护甲
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(YuraDamageStatics().ArmorDef, EvalutionParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	// 获取来源的穿透
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(YuraDamageStatics().ArmorPenetrationDef, EvalutionParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

	// 获取目标格挡概率
	float TargetBlockChance = 0.00f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(YuraDamageStatics().BlockChanceDef, EvalutionParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.00f, TargetBlockChance);
	// 计算是否格挡
	const bool bBlock = FMath::RandRange(1, 100) < TargetBlockChance * 100;


	// 伤害修改值
	const float OverArmor = (SourceArmorPenetration > TargetArmor) ? 0.f : (SourceArmorPenetration - TargetArmor);
	const float TargetDamage = bBlock ? (BaseDamage + OverArmor) * 0.5 : (BaseDamage + OverArmor);

	// 基础伤害 + 护甲穿透 - 目标护甲值
	FGameplayModifierEvaluatedData ModifierEvaluatedData_Armor(UYuraAttributeSet::GetIncomingDamageAttribute(), 
		EGameplayModOp::Additive, TargetDamage);
	OutExecutionOutput.AddOutputModifier(ModifierEvaluatedData_Armor);
}
