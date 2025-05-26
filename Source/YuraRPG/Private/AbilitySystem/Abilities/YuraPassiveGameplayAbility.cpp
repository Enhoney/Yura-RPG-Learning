// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraPassiveGameplayAbility.h"
#include "AbilitySystem/YuraAbilitySystemComponent.h"

void UYuraPassiveGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UYuraAbilitySystemComponent* YuraAsc = Cast<UYuraAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		// 绑定结束能力的回调
		YuraAsc->OnPassiveAbilityDeactive.AddUObject(this, &UYuraPassiveGameplayAbility::RecvAbilitDeactive);
	}
}

void UYuraPassiveGameplayAbility::RecvAbilitDeactive(const FGameplayTag& InAbilityTag)
{
	if (AbilityTags.HasTagExact(InAbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
