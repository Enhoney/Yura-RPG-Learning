// Copyright Muci Yun.


#include "AbilitySystem/Data/AbilityInfo.h"
#include "YuraLogChannel.h"

FYuraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bWarningIfNotFound) const
{
	for (const FYuraAbilityInfo& AbilityInfo : YuraAbilityInfos)
	{
		if (AbilityInfo.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return AbilityInfo;
		}
	}

	if (bWarningIfNotFound)
	{
		UE_LOG(LogYura, Warning, TEXT("No AbilityInfo Found For Tag : [%s]"), *AbilityTag.ToString());
	}

	return FYuraAbilityInfo();
}
