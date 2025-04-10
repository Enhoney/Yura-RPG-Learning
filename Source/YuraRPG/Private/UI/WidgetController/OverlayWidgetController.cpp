// Copyright Muci Yun.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "YuraAttributeSet.h"
#include "GameplayEffectTypes.h"

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
		YuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::BroadcastHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		YuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::BroadcasMaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		YuraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::BroadcastManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		YuraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::BroadcastMaxManaChanged);
}

void UOverlayWidgetController::BroadcastHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::BroadcasMaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::BroadcastManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::BroadcastMaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
