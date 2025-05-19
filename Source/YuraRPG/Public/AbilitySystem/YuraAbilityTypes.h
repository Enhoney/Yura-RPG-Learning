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

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC;
	
	/** 技能本身*/
	UPROPERTY()
	float AbilityBaseDamage = 0.f;

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	/** Debuff*/
	UPROPERTY()
	float DebuffChance = 0.2f;	// 触发几率

	UPROPERTY()
	float DebuffDuration = 3.f;	// 持续时间

	UPROPERTY()
	float DebuffFrequency = 0.2f;	// 频率

	UPROPERTY()
	float DebuffBaseDamage = 40.f;	// 单次触发伤害
	
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
		if (DamageTypeTag->IsValid())
		{
			return *DamageTypeTag;
		}
		return FGameplayTag();
	}

	//Set
	void SetIsDamageBlock(bool bInIsDamageBlock) { bIsDamgeBlock = bInIsDamageBlock; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

	void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	void SetDebuffBaseDamage(float InDebuffBaseDamage) { DebuffBaseDamage = InDebuffBaseDamage; }
	void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }

	void SetDamageTypeTag(const FGameplayTag& InDamageTypeTag) 
	{
		if (!DamageTypeTag->IsValid())
		{
			DamageTypeTag = MakeShared<FGameplayTag>(InDamageTypeTag);
		}
		else
		{
			*DamageTypeTag = InDamageTypeTag;
		}
		
	}

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
