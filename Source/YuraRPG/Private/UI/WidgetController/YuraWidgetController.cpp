// Copyright Muci Yun.


#include "UI/WidgetController/YuraWidgetController.h"

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
