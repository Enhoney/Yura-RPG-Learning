// Copyright Muci Yun.


#include "UI/WidgetController/YuraWidgetController.h"
#include "YuraAbilitySystemComponent.h"
#include "Data/AbilityInfo.h"

void UYuraWidgetController::SetWidgetControllrtParams(const FWidgetControllerParam& InWCParams)
{
	PlayerController = InWCParams.PlayerController;
	PlayerState = InWCParams.PlayerState;
	AbilitySystemComponent = InWCParams.AbilitySystemComponent;
	AttributeSet = InWCParams.AttributeSet;
}

void UYuraWidgetController::BroadcastInitialValues()
{
}

void UYuraWidgetController::BindCallbacksToDependiencies()
{
}

// 更新初始化技能图标
void UYuraWidgetController::BroadcastYuraAbilityInfo()
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
					YuraAbilityInfo.AbilityStatusTag = UYuraAbilitySystemComponent::GetAbilityStatusTagFromSpec(AbilitySpec);

					OnAbilityInfoGivenDelegate.Broadcast(YuraAbilityInfo);
				});

			ASC->ForEachAbility(ForEachAbilityDelegate);
		}
	}
}
