// Copyright Muci Yun.


#include "YuraAbilitySystemComponent.h"
#include "YuraGameplayTags.h"

void UYuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UYuraAbilitySystemComponent::EffectApplied);
}

void UYuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle)
{
	FGameplayTagContainer TagContainer;
	// 获取所有GE自身的Tag
	GESpec.GetAllAssetTags(TagContainer);

	OnEffectAssetTags.Broadcast(TagContainer);

}
