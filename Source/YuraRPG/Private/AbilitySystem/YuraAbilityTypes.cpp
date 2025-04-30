#include "YuraAbilityTypes.h"

bool FYuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	Super::NetSerialize(Ar, Map, bOutSuccess);

	uint8 RepBits = 0;

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
	}

	Ar.SerializeBits(&RepBits, 2);
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

	bOutSuccess = true;
	return true;

}
