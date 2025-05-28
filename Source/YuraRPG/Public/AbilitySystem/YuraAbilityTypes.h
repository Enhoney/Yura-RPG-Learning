#pragma once

#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "YuraAbilityTypes.generated.h"

class UGameplayEffect;


USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceASC;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetASC;
	
	/** 技能本身*/
	UPROPERTY(BlueprintReadWrite)
	float AbilityBaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	// 死亡冲量幅度
	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	// 击退几率
	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	// 击退力度
	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	// 死亡冲量
	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	// 击退向量
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackVector = FVector::ZeroVector;

	/** Debuff*/
	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.2f;	// 触发几率

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 3.f;	// 持续时间

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.2f;	// 频率

	UPROPERTY(BlueprintReadWrite)
	float DebuffBaseDamage = 40.f;	// 单次触发伤害

	/** 径向衰减范围伤害*/
	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamge = false;	// 是否为径向伤害

	UPROPERTY(BlueprintReadWrite)
	float RadialInnerRadius = 0.f;	// 中心半径

	UPROPERTY(BlueprintReadWrite)
	float RadialOuterRadius = 0.f;	// 伤害范围

	UPROPERTY(BlueprintReadWrite)
	FVector RadialCenterLocation = FVector::ZeroVector;	// 伤害中心点

	
};

USTRUCT(BlueprintType)
struct FYuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FYuraGameplayEffectContext::StaticStruct();
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FYuraGameplayEffectContext* Duplicate() const
	{
		FYuraGameplayEffectContext* NewContext = new FYuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

public:
	/** Custom Function*/
	// Get
	bool IsDamageBlock() const { return bIsDamgeBlock; }
	bool IsCriticalHit() const { return bIsCriticalHit; }

	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffBaseDamage() const { return DebuffBaseDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }

	FGameplayTag GetDamageTypeTag() const 
	{ 
		if (DamageTypeTag)
		{
			return *DamageTypeTag;
		}
		return FGameplayTag();
	}

	FVector GetDeathImpulse() const { return DeathImpulse; }

	FVector GetKnockbackVector() const { return KnockbackVector; }

	bool IsRadialDamage() const { return bIsRadialDamge; }
	float GetRadialInnerRadius() const { return RadialInnerRadius; }
	float GetRadialOuterRadius() const { return RadialOuterRadius; }
	FVector GetRadialCenterLocation() const { return RadialCenterLocation; }

	//Set
	void SetIsDamageBlock(bool bInIsDamageBlock) { bIsDamgeBlock = bInIsDamageBlock; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

	void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	
	void SetDebuffBaseDamage(float InDebuffBaseDamage) { DebuffBaseDamage = InDebuffBaseDamage; }
	void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }

	void SetDamageTypeTag(const FGameplayTag& InDamageTypeTag) 
	{
		if (!DamageTypeTag)
		{
			DamageTypeTag = MakeShared<FGameplayTag>(InDamageTypeTag);
		}
		else
		{
			*DamageTypeTag = InDamageTypeTag;
		}
		
	}

	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }

	void SetKnockbackVector(const FVector& InKnockbackVector) { KnockbackVector = InKnockbackVector; }


	void SetIsRadialDamage(bool bInIsRadialDamage) { bIsRadialDamge = bInIsRadialDamage; }
	void SetRadialInnerRadius(float InRadialInnerRadius) { RadialInnerRadius = InRadialInnerRadius; }
	void SetRadialOuterRadius(float InRadialOuterRadius) { RadialOuterRadius = InRadialOuterRadius; }
	void SetRadialCenterLocation(const FVector& InRadialCenterLocation) { RadialCenterLocation = InRadialCenterLocation; }

protected:

	// 是否格挡
	UPROPERTY()
	bool bIsDamgeBlock = false;

	// 是否暴击
	UPROPERTY()
	bool bIsCriticalHit = false;

	// 是否触发负面效果
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	// 负面效果参数
	UPROPERTY()
	float DebuffBaseDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamageTypeTag;

	// 这里为什么就用UPROPERTY了
	// --因为FVector就是单纯的值类型结构体，没有成员函数，基本可以当作UE基础的数据类型了
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	// 击退向量
	UPROPERTY()
	FVector KnockbackVector = FVector::ZeroVector;


	/** 范围伤害*/
	UPROPERTY()
	bool bIsRadialDamge = false;	// 是否为径向伤害

	UPROPERTY()
	float RadialInnerRadius = 0.f;	// 中心半径

	UPROPERTY()
	float RadialOuterRadius = 0.f;	// 伤害范围

	UPROPERTY()
	FVector RadialCenterLocation = FVector::ZeroVector;	// 伤害中心点

};

// USTRUCT特化
template<>
struct TStructOpsTypeTraits< FYuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FYuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
