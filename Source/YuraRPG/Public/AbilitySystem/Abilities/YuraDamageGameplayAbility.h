// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/YuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "YuraAbilityTypes.h"
#include "YuraDamageGameplayAbility.generated.h"

struct FGameplayTag;

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraDamageGameplayAbility : public UYuraGameplayAbility
{
	GENERATED_BODY()

public:
	// 生成DamageEffectParams
	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
protected:

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void GetDamageSpecHandle(FGameplayEffectSpecHandle& OutDamageEffectSpecHandle);

	// 从表格中，根据伤害类型获取当前等级的伤害
	UFUNCTION(BlueprintCallable, Category = "Ability")
	float GetBaseDamageTyped(const FGameplayTag& DamageType, int32 Level) const;

	

protected:

	// 技能伤害类型和对应的基础伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag DamageTypeTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FScalableFloat SpellBaseDamage;

	// 伤害的GE
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// 死亡冲量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float DeathImpulseMagnitude = 1000.f;

	/** Debuff Parameters*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Debuff")
	float DebuffChance = 0.2f;	// 触发几率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Debuff")
	float DebuffDuration = 3.f;	// 持续时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Debuff")
	float DebuffFrequency = 0.2f;	// 频率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Debuff")
	float DebuffBaseDamage = 40.f;	// 基础伤害--伤害结算会计算抗性

	// 击退力度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float KnockbackForceMagnitude = 1000.f;

	// 击退几率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float KnockbackChance = 0.3f;

	
};
