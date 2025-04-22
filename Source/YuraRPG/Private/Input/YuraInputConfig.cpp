// Copyright Muci Yun.


#include "Input/YuraInputConfig.h"

#include "EnhancedInput/Public/InputAction.h"

const UInputAction* UYuraInputConfig::FindInputActionByTag(const FGameplayTag& InTag, bool bLogNotFound) const
{
	for (const FYuraInputAction& YuraAction : AbilityInputActions)
	{
		if (YuraAction.InputTag.MatchesTagExact(InTag) && YuraAction.InputAction)
		{
			return YuraAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("No InputAction matching the Tag: [%s] on InputConfig: [%s]"), *InTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
