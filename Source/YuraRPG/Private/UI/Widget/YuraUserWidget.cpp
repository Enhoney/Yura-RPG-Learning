// Copyright Muci Yun.


#include "UI/Widget/YuraUserWidget.h"

void UYuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;

	WidgetControllerSet();
}
