// Copyright Muci Yun.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/AttributeSets/YuraAttributeSet.h"
#include "YuraGameplayTags.h"
#include "Player/YuraPlayerState.h"
#include "YuraAbilitySystemComponent.h"

void UAttributeMenuWidgetController::BindCallbacksToDependiencies()
{
	check(AttributeInfoData);
	if (UYuraAttributeSet* AS = Cast<UYuraAttributeSet>(AttributeSet))
	{
		for (auto& AttributePair : AS->TagsToAttributes)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributePair.Value()).AddLambda(
				[this, AttributePair](const FOnAttributeChangeData& Data) 
				{
					BrodacastAttributeInfo(AttributePair.Key, AttributePair.Value());
				}
			);
			
		}
	}

	// 更新属性点
	AYuraPlayerState* YuraPlayerState = CastChecked<AYuraPlayerState>(PlayerState);
	// 广播属性点
	YuraPlayerState->OnAttributePointChangedDelegate.AddLambda([this](int32 NewAttributePoints)
		{
			AttributePointChangedDelegate.Broadcast(NewAttributePoints);
		});

}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	AYuraPlayerState* YuraPlayerState = CastChecked<AYuraPlayerState>(PlayerState);
	if (YuraPlayerState->GetAttributePoint() > 0)
	{
		UYuraAbilitySystemComponent* YuraASC = CastChecked<UYuraAbilitySystemComponent>(AbilitySystemComponent);
		// 增加属性
		YuraASC->UpgradeAttribute(AttributeTag);
	}

}

void UAttributeMenuWidgetController::BrodacastAttributeInfo(const FGameplayTag& TagToFind, const FGameplayAttribute& Attribute)
{
	FYuraAttributeInfo AttributeInfo = AttributeInfoData->FindAttributeInfoByTag(TagToFind);
	// 获取数值
	AttributeInfo.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(AttributeInfo);
}


void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfoData);

	if (UYuraAttributeSet* AS = Cast<UYuraAttributeSet>(AttributeSet))
	{
		for (auto& AttributePair : AS->TagsToAttributes)
		{
			BrodacastAttributeInfo(AttributePair.Key, AttributePair.Value());
		}
	}

	const AYuraPlayerState* YuraPlayerState = CastChecked<AYuraPlayerState>(PlayerState);

	AttributePointChangedDelegate.Broadcast(YuraPlayerState->GetAttributePoint());

}

