// Copyright Muci Yun.


#include "YuraAbilitySystemComponent.h"
#include "YuraGameplayTags.h"
#include "Abilities/YuraGameplayAbility.h"

void UYuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UYuraAbilitySystemComponent::ClientEffectApplied);
}

void UYuraAbilitySystemComponent::GrantCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesToGrant)
	{
		// 创建Spec--可以简单理解为实例
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		// 如果这个能力是继承自我们自定义的GA类的
		if (UYuraGameplayAbility* YuraGA = Cast<UYuraGameplayAbility>(AbilitySpec.Ability))
		{
			// 动态Tag
			AbilitySpec.DynamicAbilityTags.AddTag(YuraGA->StartUpInputTag);
			// 赋予能力并不激活
			GiveAbility(AbilitySpec);
		}
	}
}

void UYuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// GetActivatableAbilities--获取所有已经注册的GAS pec
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// 这个函数的作用是这样的：
			/** 
			 * 首先，将Spec.InputPressed置为true，这个变量用于跟踪对应输入的按下状态
			 * 然后，如果这个能力被激活了，那么就根据InstancingPolicy执行对应实例--
			 * 如果是NonInstanced，就调用CDO（Spec.Ability），否则就调用Spec.GetAbilityInstances拿到所有实例
			 * 总之拿到实例后，执行Instance->InputPressed，这是一个空的虚函数，用于我们扩展的
			 * 如果我们想要在按下的时候触发某些事件，可以重写一下这个函数
			 * 这里我们没有重写，所以它的主要作用就是将Spec.InputPressed置为true
			 */
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
		
	}
}

void UYuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}

	}
}

void UYuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle)
{
	FGameplayTagContainer TagContainer;
	// 获取所有GE自身的Tag
	GESpec.GetAllAssetTags(TagContainer);

	OnEffectAssetTags.Broadcast(TagContainer);

}
