// Copyright Muci Yun.


#include "AbilitySystem/Data/AttributeInfo.h"

FYuraAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& InAttributeTag, bool bLogNotFound) const
{
	for (const FYuraAttributeInfo& Info : AttributesInformation)
	{
		// 完全匹配
		if (Info.AttributeTag.MatchesTagExact(InAttributeTag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Attribute Tag : [%s] On %s"), *InAttributeTag.ToString(), *GetNameSafe(this));
	}

	return FYuraAttributeInfo();
}
