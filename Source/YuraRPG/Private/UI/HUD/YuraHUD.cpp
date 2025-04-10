// Copyright Muci Yun.


#include "UI/HUD/YuraHUD.h"

#include "UI/Widget/YuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AYuraHUD::GetOverlayWidgetController(const FWidgetControllerParam& InWCParam)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllrtParams(InWCParam);

		// 开始监听属性变化
		OverlayWidgetController->BindCallbacksToDependiencies();
	}
	return OverlayWidgetController;
}

void AYuraHUD::InitOverlay(APlayerController* InPC, APlayerState* InPS, UAbilitySystemComponent* InASC, UAttributeSet* InAS)
{
	// 检查这两个Class有没有设置
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_YuraHUD."));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Conrtoller Class uninitialized, please fill out BP_YuraHUD."));

	OverlayWidget = CreateWidget<UYuraUserWidget>(GetWorld(), OverlayWidgetClass);

	// 初始化WC
	const FWidgetControllerParam WCParam(InPC, InPS, InASC, InAS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WCParam);
	OverlayWidget->SetWidgetController(WidgetController);

	// 广播初始值
	OverlayWidgetController->BroadcastInitialValues();

	// 添加到视口
	OverlayWidget->AddToViewport();

}