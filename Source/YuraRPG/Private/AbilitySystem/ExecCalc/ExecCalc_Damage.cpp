// Copyright Muci Yun.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AttributeSets/YuraAttributeSet.h"

#include "YuraGameplayTags.h"

#include "Data/CharacterClassInfo.h"
#include "YuraAbilitySystemLibrary.h"

#include "Interaction/CombatInterface.h"

#include "YuraAbilityTypes.h"
#include "Kismet/GameplayStatics.h"

#include "YuraLogChannel.h"

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
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	// 四抗
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightingResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	

	/** Source Attribute*/
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	// 属性和Tag的映射
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCapturedAttributeDef;

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
		DEFINE_ATTRIBUTE_CAPTUREDEF(UYuraAttributeSet, CriticalHitResistance, Target, false);
		// 四抗
		DEFINE_ATTRIBUTE_CAPTUREDEF(UYuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UYuraAttributeSet, PhysicResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UYuraAttributeSet, LightingResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UYuraAttributeSet, ArcaneResistance, Target, false);

		/** Source Attribute*/
		DEFINE_ATTRIBUTE_CAPTUREDEF(UYuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UYuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UYuraAttributeSet, CriticalHitDamage, Source, false);

		// 添加映射
		TagsToCapturedAttributeDef.Add(FYuraGameplayTags::Get().Attribute_Secondary_Armor,					ArmorDef);
		TagsToCapturedAttributeDef.Add(FYuraGameplayTags::Get().Attribute_Secondary_BlockChance,			BlockChanceDef);
		TagsToCapturedAttributeDef.Add(FYuraGameplayTags::Get().Attribute_Secondary_CriticalHitResistance,	CriticalHitResistanceDef);
		TagsToCapturedAttributeDef.Add(FYuraGameplayTags::Get().Attribute_Resilience_Fire,					FireResistanceDef);
		TagsToCapturedAttributeDef.Add(FYuraGameplayTags::Get().Attribute_Resilience_Physic,				PhysicResistanceDef);
		TagsToCapturedAttributeDef.Add(FYuraGameplayTags::Get().Attribute_Resilience_Lighting,				LightingResistanceDef);
		TagsToCapturedAttributeDef.Add(FYuraGameplayTags::Get().Attribute_Resilience_Arcane,				ArcaneResistanceDef);

		TagsToCapturedAttributeDef.Add(FYuraGameplayTags::Get().Attribute_Secondary_ArmorPenetration,		ArmorPenetrationDef);
		TagsToCapturedAttributeDef.Add(FYuraGameplayTags::Get().Attribute_Secondary_CriticalHitChance,		CriticalHitChanceDef);
		TagsToCapturedAttributeDef.Add(FYuraGameplayTags::Get().Attribute_Secondary_CriticalHitDamage,		CriticalHitDamageDef);



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
	RelevantAttributesToCapture.Add(YuraDamageStatics().CriticalHitResistanceDef);
	//四抗
	RelevantAttributesToCapture.Add(YuraDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(YuraDamageStatics().PhysicResistanceDef);
	RelevantAttributesToCapture.Add(YuraDamageStatics().LightingResistanceDef);
	RelevantAttributesToCapture.Add(YuraDamageStatics().ArcaneResistanceDef);

	/** Source Attribute*/
	RelevantAttributesToCapture.Add(YuraDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(YuraDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(YuraDamageStatics().CriticalHitDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	
	// 获取ASC
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	// 拿到对应的AvatorActor，一般就是Pawn
	AActor* SourceAvatorActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatortActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvalutionParameters;
	EvalutionParameters.SourceTags = SourceTags;
	EvalutionParameters.TargetTags = TargetTags;

	// 根据Spec拿到Context
	FGameplayEffectContextHandle ContextHandle = Spec.GetEffectContext();

	// Debuff计算
	DeterminingDebuff(Spec, ExecutionParams, EvalutionParameters);

	// 通过SetByCaller拿到技能基础伤害
	float BaseDamage = 0.f;

	for (const auto& DamageTypeToResistance : FYuraGameplayTags::Get().DamageTypeToResistanceTags)
	{
		const FGameplayTag DamageType = DamageTypeToResistance.Key;
		const FGameplayTag AttributeTag = DamageTypeToResistance.Value;

		checkf(FYuraDamageStatics().TagsToCapturedAttributeDef.Contains(AttributeTag), TEXT("Attribute Mapping for Tag [%s] has not captured!!"), *AttributeTag.ToString());

		// 拿到对应的属性
		float RealResistence = 0.f;
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = FYuraDamageStatics().TagsToCapturedAttributeDef[AttributeTag];
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvalutionParameters, RealResistence);
		RealResistence = FMath::Clamp(RealResistence, 0.00f, 1.00f);

		// 拿到对应属性的技能基础伤害
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeToResistance.Key, false);

		// 省去无意义的操作
		if (DamageTypeValue <= 0.f)
		{
			continue;
		}
		
		if (ICombatInterface* TargetInterface = Cast<ICombatInterface>(TargetAvatortActor))
		{
			TargetInterface->GetDamageTakenDelegate().AddLambda([&](float DamageAmount) {
				DamageTypeValue = DamageAmount;
				});
		}

		// 这里的用法及其烧脑，为什么可以这样用，在Apply之后，这里会等吗？
		UGameplayStatics::ApplyRadialDamageWithFalloff(
			TargetAvatortActor,
			DamageTypeValue,
			0.f,
			UYuraAbilitySystemLibrary::GetRadialCenterLocation(ContextHandle),
			UYuraAbilitySystemLibrary::GetRadialInnerRadius(ContextHandle),
			UYuraAbilitySystemLibrary::GetRadialOuterRadius(ContextHandle),
			1.f,
			UDamageType::StaticClass(),
			TArray<AActor*>(),
			SourceAvatorActor);

		BaseDamage += DamageTypeValue * (1 - RealResistence);	
		
	}

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

	// 获取穿透系数
	UCharacterClassInfo* CharacterClassInfo = UYuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatorActor);
	// 找到对应的曲线
	FRealCurve* CoefficientCurve = CharacterClassInfo->DamageCalcCoefTable->FindCurve(FName("Coeffiicient.ArmorPene"), FString());
	// 拿到来源角色等级
	ICombatInterface* SourceCombat = Cast<ICombatInterface>(SourceAvatorActor);
	float SourceCharacterLevel = 1.f;
	if (SourceCombat)
	{
		SourceCharacterLevel = SourceCombat->GetCharacterLevel();
	}
	// 通过角色等级拿到曲线上的值
	float DamageArmorPeneCoef =  CoefficientCurve->Eval(SourceCharacterLevel);

	// 伤害修改值，穿透计算的时候，加上系数
	const float OverArmor = DamageArmorPeneCoef * SourceArmorPenetration - TargetArmor;
	// 要修改的目标伤害
	float TargetDamage = bBlock ? (BaseDamage + OverArmor) * 0.5 : (BaseDamage + OverArmor);

	/** 暴击计算*/
	// 来源的暴击率
	float SourceCriticalHitChance = 0.00f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(YuraDamageStatics().CriticalHitChanceDef, EvalutionParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.00f, SourceCriticalHitChance);
	// 来源的暴击伤害
	float SourceCriticalHitDamage = 0.00f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(YuraDamageStatics().CriticalHitDamageDef, EvalutionParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.00f, SourceCriticalHitDamage);
	// 目标的暴击抗性
	float TargetCriticalHitResistance = 0.00f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(YuraDamageStatics().CriticalHitResistanceDef, EvalutionParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.00f, TargetCriticalHitResistance);
	// 执行计算
	bool bCriticalHit = CalculCriticalHitDamage(TargetDamage, SourceCriticalHitChance, TargetCriticalHitResistance, SourceCriticalHitDamage);

	// 设置自定义Context参数
	// 首先需要拿到Context，并转换为我们自定义的类型
	// 这里返回的是引用，所以可以直接使用
	UYuraAbilitySystemLibrary::SetDamageBlock(ContextHandle, bBlock);
	UYuraAbilitySystemLibrary::SetCriticalHit(ContextHandle, bCriticalHit);

	// 基础伤害 + 护甲穿透 - 目标护甲值
	FGameplayModifierEvaluatedData ModifierEvaluatedData_Armor(UYuraAttributeSet::GetIncomingDamageAttribute(), 
		EGameplayModOp::Additive, TargetDamage > 0 ? TargetDamage : 0);
	OutExecutionOutput.AddOutputModifier(ModifierEvaluatedData_Armor);
}

void UExecCalc_Damage::DeterminingDebuff(const FGameplayEffectSpec& Spec, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters& EvalutionParameters) const
{
	const FYuraGameplayTags YuraTags = FYuraGameplayTags::Get();
	for (TTuple<FGameplayTag, FGameplayTag> Pair : YuraTags.DamageTypeToDebuff)
	{
		const FGameplayTag DamageType = Pair.Key;
		const FGameplayTag DebuffType = Pair.Value;

		// 找到基础伤害，如果这个技能有基础伤害，那么就可以施加Debuff--属性对应上了
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if (DamageTypeValue > .5f)
		{
			// 得到基础施加Debuff成功率
			const float SourceDeuffChance = Spec.GetSetByCallerMagnitude(YuraTags.DebuffParam_Chance, false, -1.f);

			// 找对应的属性抗性
			float TargetResistance = 0.f;
			const FGameplayTag TargetResistanceTag = YuraTags.DamageTypeToResistanceTags[DamageType];
			const FGameplayEffectAttributeCaptureDefinition CaptureDef = FYuraDamageStatics().TagsToCapturedAttributeDef[TargetResistanceTag];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvalutionParameters, TargetResistance);
			TargetResistance = FMath::Max(0.f, TargetResistance);	// 这个抗性一定是小于1，大于0的

			// 计算真实的施加Debuff几率
			const float RealDebuffChance = SourceDeuffChance * (1.f - TargetResistance);

			const bool bDebuffApply = FMath::RandRange(1, 100) < RealDebuffChance * 100;

			if (bDebuffApply)
			{
				// 这就表明成功施加负面效果了
				// 从Spec中拿到DebuffInfo：
				const float SourceDebuffBaseDamage = Spec.GetSetByCallerMagnitude(YuraTags.DebuffParam_BaseDamage, false);
				const float SourceDebuffDuration = Spec.GetSetByCallerMagnitude(YuraTags.DebuffParam_Duration, false);
				const float SourceDebuffFrequency = Spec.GetSetByCallerMagnitude(YuraTags.DebuffParam_Frequency, false);
				// 然后设置DebuffInfo到Context中：
				FGameplayEffectContextHandle ContextHandle = Spec.GetEffectContext();
				UYuraAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, bDebuffApply);
				UYuraAbilitySystemLibrary::SetDebuffBaseDamage(ContextHandle, SourceDebuffBaseDamage);
				UYuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, SourceDebuffDuration);
				UYuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, SourceDebuffFrequency);
				UYuraAbilitySystemLibrary::SetDamageTypeTag(ContextHandle, DamageType);

			}

		}
	}
}

bool UExecCalc_Damage::CalculCriticalHitDamage(float& BaseDamage, const float SourceHitCriticalChance, const float TargetHitCriticalRes, const float SourceHitCriticalDamage) const
{
	const bool bHitCriticl = FMath::RandRange(1, 100) < ((SourceHitCriticalChance - TargetHitCriticalRes) * 100);

	if (bHitCriticl)
	{
		BaseDamage *= SourceHitCriticalDamage;
	}

	return bHitCriticl;
}
