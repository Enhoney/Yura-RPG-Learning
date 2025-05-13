// Copyright Muci Yun.


#include "YuraAbilitySystemComponent.h"
#include "YuraGameplayTags.h"
#include "Abilities/YuraGameplayAbility.h"
#include "YuraLogChannel.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/PlayerInterface.h"

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

	bStartupAbilitiesGiven = true;
	OnAbilitiesGivenDelegate.Broadcast();
}

void UYuraAbilitySystemComponent::GrantCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilitiesToGrant)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : PassiveAbilitiesToGrant)
	{
		// 创建Spec--可以简单理解为实例
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		// 赋予能力并立即激活
		GiveAbilityAndActivateOnce(AbilitySpec);
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

void UYuraAbilitySystemComponent::ForEachAbility(const FForEachAbilitySignature& Delegate)
{
	FScopedAbilityListLock ActivateAbilityLock(*this);

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogYura, Error, TEXT("Failed to Execute delegate in [%s]"), *FString(__FUNCTION__));
		}
	}
}

FGameplayTag UYuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		FGameplayTagContainer AbilityTagContainer = AbilitySpec.Ability.Get()->AbilityTags;
		for (FGameplayTag Tag : AbilityTagContainer)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability"))))
			{
				return Tag;
			}
		}
	}

	return FGameplayTag();
}

FGameplayTag UYuraAbilitySystemComponent::GetAbilityInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

void UYuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		// 第一层判断--客户端
		if (IPlayerInterface::Execute_GetAttributePoint(GetAvatarActor()) > 0)
		{
			// 执行RPC，在服务器继续走
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UYuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		// 第二层判断--服务器
		if (IPlayerInterface::Execute_GetAttributePoint(GetAvatarActor()) > 0)
		{
			FGameplayEventData AttributeEventData;
			AttributeEventData.EventTag = AttributeTag;
			AttributeEventData.EventMagnitude = 1.0;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, AttributeEventData);

			// 消耗属性点
			IPlayerInterface::Execute_ConsumeAttributePoint(GetAvatarActor(), 1);
		}
	}
}

void UYuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	// 我们目前只想在一开始的时候执行一次广播
	// bStartupAbilitiesGiven这个属性没有同步，所以这里客户端一开始时false
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		OnAbilitiesGivenDelegate.Broadcast();
	}
	
}

void UYuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle)
{
	FGameplayTagContainer TagContainer;
	// 获取所有GE自身的Tag
	GESpec.GetAllAssetTags(TagContainer);

	OnEffectAssetTags.Broadcast(TagContainer);

}
