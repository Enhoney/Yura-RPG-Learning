#pragma once

#include "GameplayEffectTypes.h"

#include "YuraAbilityTypes.generated.h"

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
	//Set
	void SetIsDamageBlock(bool bInIsDamageBlock) { bIsDamgeBlock = bInIsDamageBlock; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

protected:

	// 是否格挡
	UPROPERTY()
	bool bIsDamgeBlock = false;

	// 是否暴击
	UPROPERTY()
	bool bIsCriticalHit = false;


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
