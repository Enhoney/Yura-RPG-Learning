// Copyright Muci Yun.


#include "YuraAbilitySystemComponent.h"
#include "YuraGameplayTags.h"
#include "Abilities/YuraGameplayAbility.h"
#include "YuraLogChannel.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/PlayerInterface.h"
#include "Data/AbilityInfo.h"
#include "AbilitySystem/YuraAbilitySystemLibrary.h"

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
			AbilitySpec.DynamicAbilityTags.AddTag(FYuraGameplayTags::Get().Ability_Status_Equipped);
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

	// GetActivatableAbilities--获取所有已经注册的GAS spec
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

void UYuraAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	// 首先拿到在GameMode中的配置信息
	UAbilityInfo* AbilityInfo = UYuraAbilitySystemLibrary::GetAbilityInfoOnGameMode(GetAvatarActor());

	check(AbilityInfo);

	for (const FYuraAbilityInfo& Info : AbilityInfo->YuraAbilityInfos)
	{
		const int32 LevelRequirement = Info.LevelRequirement;
		// 如果没有配置AbilityTag，也直接继续
		if (!Info.AbilityTag.IsValid())
		{
			continue;
		}
		// 不满足激活条件，就继续遍历
		if (Level < LevelRequirement)
		{
			continue;
		}
		else
		{
			// 如果满足解锁条件，但是能力还没有赋予
			if (GetSpecByAbilityTag(Info.AbilityTag) == nullptr)
			{
				// 就赋予这个能力
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);
				// 将状态设为可解锁
				const FGameplayTag NewStatusTag = FYuraGameplayTags::Get().Ability_Status_Eligible;
				AbilitySpec.DynamicAbilityTags.AddTag(NewStatusTag);
				// 赋予能力
				GiveAbility(AbilitySpec);
				// 强制他立即复制，而不是等待下一次状态同步
				MarkAbilitySpecDirty(AbilitySpec);
				// RPC--告知客户端更新技能树
				ClientAbilityStatusesChanged(Info.AbilityTag, NewStatusTag);
			}
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

FGameplayTag UYuraAbilitySystemComponent::GetAbilityStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability.Status"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

FGameplayTag UYuraAbilitySystemComponent::GetAbilityTypeTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability.Type"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

FGameplayAbilitySpec* UYuraAbilitySystemComponent::GetSpecByAbilityTag(const FGameplayTag& AbilityTag)
{
	// 避免有能力被移除或者添加
	FScopedAbilityListLock AbilityLock(*this);

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		for (FGameplayTag YuraAbilityTag : Spec.Ability->AbilityTags)
		{
			if (YuraAbilityTag.MatchesTagExact(AbilityTag))
			{
				return &Spec;
			}
		}
	}

	return nullptr;
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

void UYuraAbilitySystemComponent::ClientAbilityStatusesChanged_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& NewStatusTag)
{
	OnAbilityStatusChangedDelegate.Broadcast(AbilityTag, NewStatusTag);
}

