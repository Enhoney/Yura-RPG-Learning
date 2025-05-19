// Copyright Muci Yun.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "YuraAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "YuraAbilitySystemComponent.h"

#include "Engine/DataTable.h"

#include "Player/YuraPlayerState.h"
#include "Player/Data/LevelUpInfo.h"

#include "Data/AbilityInfo.h"
#include "YuraGameplayTags.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UYuraAttributeSet* YuraAttributeSet = CastChecked<UYuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(YuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(YuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(YuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(YuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependiencies()
{

	AYuraPlayerState* YuraPlayerState = CastChecked<AYuraPlayerState>(PlayerState);

	// 绑定等级和经验提升回调
	YuraPlayerState->OnPlayerExpChangedDelegate.AddUObject(this, &ThisClass::OnExpChanged);
	YuraPlayerState->OnPlayerLevelChangedDelegate.AddUObject(this, &ThisClass::OnPlayerLevelChanged);

	const UYuraAttributeSet* YuraAttributeSet = CastChecked<UYuraAttributeSet>(AttributeSet);

	// 绑定回调
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		YuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		YuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		YuraAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		YuraAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			});

	if (UYuraAbilitySystemComponent* ASC = Cast<UYuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		
		if (ASC->bStartupAbilitiesGiven)
		{
			BroadcastYuraAbilityInfo();
		}
		else
		{
			ASC->OnAbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastYuraAbilityInfo);
		}

		ASC->OnEffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& InGameplayTagContainer)
			{
				for (const FGameplayTag& TmpTag : InGameplayTagContainer)
				{
					// 这会去项目配置中找这个Tag，找不到默认会报错
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					// 只要包含就会是true
					bool bIsMessageTag = TmpTag.MatchesTag(MessageTag);

					if (bIsMessageTag)
					{
						FUIWidgeRow* RowData = GetDataTableRowByTag<FUIWidgeRow>(TmpTag, MessageTable.Get());

						OnMessageWidgetRowDelegate.Broadcast(*RowData);
					}

				}
			});

		// 更新装备状态
		ASC->OnAbilityEquipAndUnloadDelegate.AddUObject(this, &UOverlayWidgetController::EquipAbilityCallback);
	}

}

void UOverlayWidgetController::OnPlayerLevelChanged(int32 NewLevel)
{
	OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
}

void UOverlayWidgetController::OnExpChanged(int32 NewExp)
{
	AYuraPlayerState* YuraPlayerState = CastChecked<AYuraPlayerState>(PlayerState);

	const ULevelUpInfo* LevelUpInfo = YuraPlayerState->LevelUpInfo;

	checkf(LevelUpInfo, TEXT("Unable to find valid LevelUpInfo on PlayerState!!!"));
	// 根据经验找等级
	const int32 CurrentLevel = LevelUpInfo->FindCurrentLevelByExp(NewExp);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num() - 1;	// 因为有一个占位的--0级，所以里面会有51个
	// 保证当前等级是有效的
	if (CurrentLevel <= MaxLevel && CurrentLevel > 0)
	{
		// 根据等级找到升级需要的经验
		const int32 RequiredExp = LevelUpInfo->LevelUpInformation[CurrentLevel].ExpRequirement;
		// 找到前一级升级所需经验，他们相减就是我们这一等级升级需要的经验了
		const int32 PreviousRequiredExp = LevelUpInfo->LevelUpInformation[CurrentLevel - 1].ExpRequirement;
		// 当前等级升级所需的经验
		const int32 DeltaRequiredExp = RequiredExp - PreviousRequiredExp;
		// 当前等级升级已经获取的经验
		const int32 DeltaCurExp = NewExp - PreviousRequiredExp;

		// 百分比
		const float ExpPercent = static_cast<float>(DeltaCurExp) / static_cast<float>(DeltaRequiredExp);

		OnExpChangedDelegate.Broadcast(ExpPercent);
	}
	
}

void UOverlayWidgetController::EquipAbilityCallback(const FGameplayTag& AbilityTag, const FGameplayTag& NewStatusTag, 
	const FGameplayTag& InputSlot, const FGameplayTag& PreInputSlot, bool bIsAbilitySwap)
{
	check(AbilityInformations);

	// 如果不是交换键位
	if (!bIsAbilitySwap)
	{
		// 得先清除原来的
		FYuraAbilityInfo PreYuraAbilityInfo = FYuraAbilityInfo();
		PreYuraAbilityInfo.AbilityTag = FGameplayTag();
		PreYuraAbilityInfo.AbilityInputTag = PreInputSlot;
		PreYuraAbilityInfo.AbilityStatusTag = FYuraGameplayTags::Get().Ability_Status_Unlocked;
		OnAbilityInfoGivenDelegate.Broadcast(PreYuraAbilityInfo);
	}

	// 再来广播当前的
	FYuraAbilityInfo YuraAbilityInfo = AbilityInformations->FindAbilityInfoByTag(AbilityTag);
	// 这个有可能是空的
	YuraAbilityInfo.AbilityInputTag = InputSlot;
	YuraAbilityInfo.AbilityStatusTag = NewStatusTag;

	OnAbilityInfoGivenDelegate.Broadcast(YuraAbilityInfo);

}
