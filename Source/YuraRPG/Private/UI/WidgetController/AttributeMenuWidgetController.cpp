// Copyright Muci Yun.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/AttributeSets/YuraAttributeSet.h"
#include "YuraGameplayTags.h"

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

}

