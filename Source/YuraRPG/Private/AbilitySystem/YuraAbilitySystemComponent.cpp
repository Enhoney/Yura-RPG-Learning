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

void UYuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// GetActivatableAbilities--获取所有已经注册的GAS spec
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{

			AbilitySpecInputPressed(AbilitySpec);
			// 向服务器发送数据，告诉本地对应的按键已经按下了
			
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
			break;
		}

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
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);

			// 向服务器发送信息，告诉它我们本地已经释放按钮了
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
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
				ClientAbilityStatusesChanged(Info.AbilityTag, NewStatusTag, AbilitySpec.Level);
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

bool UYuraAbilitySystemComponent::GetAbilityCurrentDescription(const FGameplayTag& InAbilityTag, const class UAbilityInfo* InAbilityInfo, FString& OutCurDescription, FString& OutNextLevelDescription)
{
	// 先看看这个能力是否有被赋予
	FGameplayAbilitySpec* AbilitySpec = GetSpecByAbilityTag(InAbilityTag);

	if (!AbilitySpec)
	{
		check(InAbilityInfo);

		FYuraAbilityInfo YuraAbilityInfo = InAbilityInfo->FindAbilityInfoByTag(InAbilityTag);

		OutCurDescription = UYuraGameplayAbility::GetLockedDescription(YuraAbilityInfo.LevelRequirement);
		OutNextLevelDescription = FString();

		return false;
	}

	if (UYuraGameplayAbility* YuraAbility = Cast<UYuraGameplayAbility>(AbilitySpec->Ability))
	{
		OutCurDescription = YuraAbility->GetCurrentLevelDescription(AbilitySpec->Level);
		OutNextLevelDescription = YuraAbility->GetNextLevelDescription(AbilitySpec->Level + 1);

		return true;
	}

	return false;
}

FGameplayTag UYuraAbilitySystemComponent::GetStatusByAbilityTag(const FGameplayTag& InAbilityTag)
{
	// 避免有能力被移除或者添加
	FScopedAbilityListLock AbilityLock(*this);

	FGameplayTag OutStatusTag = FGameplayTag();

	// 找到能力
	FGameplayAbilitySpec* AbilitySpec = GetSpecByAbilityTag(InAbilityTag);
	if (AbilitySpec)
	{
		// 如果这个能力已经被赋予，就拿到上面的StatusTag
		OutStatusTag = UYuraAbilitySystemComponent::GetAbilityStatusTagFromSpec(*AbilitySpec);
	}

	return OutStatusTag;
}

FGameplayTag UYuraAbilitySystemComponent::GetInputByAbilityTag(const FGameplayTag& InAbilityTag)
{
	// 避免有能力被移除或者添加
	FScopedAbilityListLock AbilityLock(*this);

	FGameplayTag OutInputTag = FGameplayTag();

	// 找到能力
	FGameplayAbilitySpec* AbilitySpec = GetSpecByAbilityTag(InAbilityTag);
	if (AbilitySpec)
	{
		// 如果这个能力已经被赋予，就拿到上面的StatusTag
		OutInputTag = UYuraAbilitySystemComponent::GetAbilityInputTagFromSpec(*AbilitySpec);
	}

	return OutInputTag;
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

FGameplayAbilitySpec* UYuraAbilitySystemComponent::GetSpecByAbilityInputTag(const FGameplayTag& AbilityInputTag)
{
	// 避免有能力被移除或者添加
	FScopedAbilityListLock AbilityLock(*this);

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		for (FGameplayTag YuraAbilityTag : Spec.DynamicAbilityTags)
		{
			if (YuraAbilityTag.MatchesTagExact(AbilityInputTag))
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

void UYuraAbilitySystemComponent::ServerSpendignSpellPoint_Implementation(const FGameplayTag& AbilityTag, int32 SpellPointsToSpend)
{
	// 首先找到这个能力
	if (FGameplayAbilitySpec* AblitySpec = GetSpecByAbilityTag(AbilityTag))
	{

		const FYuraGameplayTags YuraGameplayTags = FYuraGameplayTags::Get();

		// 然后查看它的状态
		FGameplayTag AbilityStatus = GetAbilityStatusTagFromSpec(*AblitySpec);

		// 一般来说，这个肯定是有效的
		if (AbilityStatus.IsValid())
		{
			// 消耗技能点
			if (GetAvatarActor()->Implements<UPlayerInterface>())
			{
				IPlayerInterface::Execute_ConsumeSpellPoint(GetAvatarActor(), SpellPointsToSpend);
			}

			if (AbilityStatus.MatchesTagExact((YuraGameplayTags.Ability_Status_Eligible)))
			{
				// 解锁技能--修改状态为Unlock
				// 删除原有StatusTag，添加新的Tag
				AblitySpec->DynamicAbilityTags.RemoveTag(YuraGameplayTags.Ability_Status_Eligible);
				AblitySpec->DynamicAbilityTags.AddTag(YuraGameplayTags.Ability_Status_Unlocked);
				// 通知客户端这个能力的状态修改了，可以更新UI了
				ClientAbilityStatusesChanged(AbilityTag, YuraGameplayTags.Ability_Status_Unlocked, AblitySpec->Level);
				
			}
			else if (AbilityStatus.MatchesTagExact((YuraGameplayTags.Ability_Status_Unlocked)) ||
				AbilityStatus.MatchesTagExact((YuraGameplayTags.Ability_Status_Equipped)))
			{
				// 升级技能，不修改状态
				AblitySpec->Level += 1;
				ClientAbilityStatusesChanged(AbilityTag, AbilityStatus, AblitySpec->Level);
			}

			// 强制更新GA到客户端
			MarkAbilitySpecDirty(*AblitySpec);
		}
	}

}

void UYuraAbilitySystemComponent::ServerEquipSpellToInputSlot_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& TargetInputTag)
{
	// 避免有能力被移除或者添加
	FScopedAbilityListLock AbilityLock(*this);

	bool bIsAbilitySwap = false;

	// 如果这个技能压根就没被赋予--一般来说不会有这种情况，除非UI上面配置错了，并且上游逻辑有问题
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecByAbilityTag(AbilityTag))
	{
		// 获取现在的InputTag
		const FGameplayTag PreIputTag = GetAbilityInputTagFromSpec(*AbilitySpec);
		// 获取现在的状态
		const FGameplayTag PreStatusTag = GetAbilityStatusTagFromSpec(*AbilitySpec);

		const FYuraGameplayTags YuraTags = FYuraGameplayTags::Get();
		const bool bStatusValid = PreStatusTag.MatchesTagExact(YuraTags.Ability_Status_Unlocked) || 
			PreStatusTag.MatchesTagExact(YuraTags.Ability_Status_Equipped);
		if (!bStatusValid)
		{
			return;
		}
		// 如果当前槽位装备了技能，就卸载原来的技能，并装备到现在这个槽位
		// 那么就需要找到原来的技能了
		FGameplayAbilitySpec* PreAbilitySpec = GetSpecByAbilityInputTag(TargetInputTag);
		// 如果原来在这个槽上装备了技能，就先卸载它，并执行广播，没有就直接装备新技能
		if (PreAbilitySpec)
		{
			UnloadAbilityEquipped(PreAbilitySpec);
			if (PreIputTag.IsValid())
			{
				// 如果要装备的那个技能原来的槽不是空的
				// 就交换
				EquipAbility(PreAbilitySpec, PreIputTag);
				bIsAbilitySwap = true;
			}
			// 立即执行复制
			MarkAbilitySpecDirty(*PreAbilitySpec);
			const FGameplayTag PreEquippedAbilityTag = GetAbilityTagFromSpec(*PreAbilitySpec);
			ClientEquipAbility(PreEquippedAbilityTag, UYuraAbilitySystemComponent::GetAbilityStatusTagFromSpec(*PreAbilitySpec), PreIputTag, TargetInputTag, bIsAbilitySwap);
		}
		// 装备新技能
		EquipAbility(AbilitySpec, TargetInputTag);
		MarkAbilitySpecDirty(*AbilitySpec);
		ClientEquipAbility(AbilityTag, YuraTags.Ability_Status_Equipped, TargetInputTag, PreIputTag, bIsAbilitySwap);
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

void UYuraAbilitySystemComponent::UnloadAbilityEquipped(FGameplayAbilitySpec* AbilitySpec)
{
	// 避免有能力被移除或者添加
	FScopedAbilityListLock AbilityLock(*this);

	// 如果是被动技能，就结束它
	if (IsPassiveAbility(*AbilitySpec))
	{
		UnloadPassiveAbilityEquipped(GetAbilityTagFromSpec(*AbilitySpec));
	}

	// 修改状态
	AbilitySpec->DynamicAbilityTags.RemoveTag(FYuraGameplayTags::Get().Ability_Status_Equipped);
	AbilitySpec->DynamicAbilityTags.AddTag(FYuraGameplayTags::Get().Ability_Status_Unlocked);
	// 清除InputTag
	const FGameplayTag InputSolt = GetAbilityInputTagFromSpec(*AbilitySpec);
	AbilitySpec->DynamicAbilityTags.RemoveTag(InputSolt);
}

bool UYuraAbilitySystemComponent::EquipAbility(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& TargetInputTag)
{
	// 避免有能力被移除或者添加
	FScopedAbilityListLock AbilityLock(*this);

	// 标记这个技能是否装备上了--只有当前后槽位一样的情况下才会装备失败
	bool bNewEquip = false;

	// 修改状态--如果没有装备上的话
	if (!AbilitySpec->DynamicAbilityTags.HasTagExact(FYuraGameplayTags::Get().Ability_Status_Equipped))
	{
		// 如果是被动技能，就激活它
		if (IsPassiveAbility(*AbilitySpec))
		{
			EquipPassiveAbility(GetAbilityTagFromSpec(*AbilitySpec));
		}

		// 只有解锁状态才能点击装备，所以这个技能一定至少是解锁状态
		AbilitySpec->DynamicAbilityTags.RemoveTag(FYuraGameplayTags::Get().Ability_Status_Unlocked);
		AbilitySpec->DynamicAbilityTags.AddTag(FYuraGameplayTags::Get().Ability_Status_Equipped);
	}
	
	// 清除原来的InputTag--如果原来已经装备过了
	const FGameplayTag PreInputSolt = GetAbilityInputTagFromSpec(*AbilitySpec);
	// 如果两个输入位置是不一样的
	if (PreInputSolt.IsValid() && !PreInputSolt.MatchesTagExact(TargetInputTag))
	{
		AbilitySpec->DynamicAbilityTags.RemoveTag(PreInputSolt);
		// 赋予新的输入
		AbilitySpec->DynamicAbilityTags.AddTag(TargetInputTag);

		bNewEquip = true;
	}
	else if (!PreInputSolt.IsValid())
	{
		// 赋予新的输入
		AbilitySpec->DynamicAbilityTags.AddTag(TargetInputTag);

		bNewEquip = true;
	}

	return bNewEquip;
}

void UYuraAbilitySystemComponent::UnloadPassiveAbilityEquipped(const FGameplayTag& AbilityTag)
{
	OnPassiveAbilityDeactive.Broadcast(AbilityTag);
}

bool UYuraAbilitySystemComponent::EquipPassiveAbility(const FGameplayTag& AbilityTag)
{
	FGameplayTagContainer PassiveAbilityTags;
	PassiveAbilityTags.AddTag(AbilityTag);

	return TryActivateAbilitiesByTag(PassiveAbilityTags);
}

bool UYuraAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec(AbilitySpec);

	// 首先拿到在GameMode中的配置信息
	UAbilityInfo* AbilityInfo = UYuraAbilitySystemLibrary::GetAbilityInfoOnGameMode(GetAvatarActor());
	check(AbilityInfo);
	FYuraAbilityInfo YuraAbilityInfo = AbilityInfo->FindAbilityInfoByTag(AbilityTag);

	const FGameplayTag PassiveTag = FYuraGameplayTags::Get().Ability_Type_Passive;

	if (PassiveTag.IsValid())
	{
		return YuraAbilityInfo.AbilityTypeTag.MatchesTag(PassiveTag);
	}
	return false;
}

void UYuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle)
{
	FGameplayTagContainer TagContainer;
	// 获取所有GE自身的Tag
	GESpec.GetAllAssetTags(TagContainer);

	OnEffectAssetTags.Broadcast(TagContainer);

}

void UYuraAbilitySystemComponent::ClientAbilityStatusesChanged_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& NewStatusTag, int32 NewAbilityLevel)
{
	OnAbilityStatusChangedDelegate.Broadcast(AbilityTag, NewStatusTag, NewAbilityLevel);
}

void UYuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& NewStatusTag, 
	const FGameplayTag& InputSlot, const FGameplayTag& PreInputSlot, bool bIsAbilitySwap)
{
	OnAbilityEquipAndUnloadDelegate.Broadcast(AbilityTag, NewStatusTag, InputSlot, PreInputSlot, bIsAbilitySwap);
}

