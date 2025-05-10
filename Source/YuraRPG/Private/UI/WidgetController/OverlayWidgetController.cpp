// Copyright Muci Yun.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "YuraAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "YuraAbilitySystemComponent.h"

#include "Engine/DataTable.h"
#include "Data/AbilityInfo.h"

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
			OnStartupAbilitiesGiven();
		}
		else
		{
			ASC->OnAbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnStartupAbilitiesGiven);
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
	}

}

void UOverlayWidgetController::OnStartupAbilitiesGiven()
{
	if (UYuraAbilitySystemComponent* ASC = Cast<UYuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (ASC->bStartupAbilitiesGiven)
		{
			FForEachAbilitySignature ForEachAbilityDelegate;

			ForEachAbilityDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
				{
					FYuraAbilityInfo YuraAbilityInfo = AbilityInformations->FindAbilityInfoByTag(UYuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
					YuraAbilityInfo.AbilityInputTag = UYuraAbilitySystemComponent::GetAbilityInputTagFromSpec(AbilitySpec);

					OnAbilityInfoGivenDelegate.Broadcast(YuraAbilityInfo);
				});

			ASC->ForEachAbility(ForEachAbilityDelegate);
		}
	}
}
