#include "YuraAbilityTypes.h"

bool FYuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	Super::NetSerialize(Ar, Map, bOutSuccess);

	uint32 RepBits = 0;

	if (Ar.IsSaving())
	{
		// 自定义的内容了，如果是false，就不用复制了
		// 因为默认就是0
		if (bIsDamgeBlock)
		{
			RepBits |= 1 << 0;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 1;
		}
		if (bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 2;
		}
		if (DebuffBaseDamage > 0.f)
		{
			RepBits |= 1 << 3;
		}
		if (DebuffDuration > 0.f)
		{
			RepBits |= 1 << 4;
		}
		if (DebuffFrequency > 0.f)
		{
			RepBits |= 1 << 5;
		}
		if (DamageTypeTag)
		{
			RepBits |= 1 << 6;
		}
		if (!DeathImpulse.IsZero())
		{
			RepBits |= 1 << 7;
		}
		if (!KnockbackVector.IsZero())
		{
			RepBits |= 1 << 8;
		}
	}

	Ar.SerializeBits(&RepBits, 9);
	// 是否格挡
	if (RepBits & (1 << 0))
	{
		Ar << bIsDamgeBlock;
	}
	// 是否暴击
	if (RepBits & (1 << 1))
	{
		Ar << bIsCriticalHit;
	}
	// 是否施加负面效果
	if (RepBits & (1 << 2))
	{
		Ar << bIsSuccessfulDebuff;
	}
	if (RepBits & (1 << 3))
	{
		Ar << DebuffBaseDamage;
	}
	if (RepBits & (1 << 4))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 5))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 6))
	{
		if (Ar.IsLoading())
		{
			if (!DamageTypeTag.IsValid())
			{
				DamageTypeTag = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		DamageTypeTag->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 7))
	{
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 8))
	{
		KnockbackVector.NetSerialize(Ar, Map, bOutSuccess);
	}

	bOutSuccess = true;
	return true;

}
