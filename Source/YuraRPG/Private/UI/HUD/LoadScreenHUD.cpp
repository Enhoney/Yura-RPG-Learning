// Copyright Muci Yun.


#include "UI/HUD/LoadScreenHUD.h"

#include "UI/Widget/LoadScreenUserWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	// 实例化MVVM
	LoadScreenModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenModelClass);
	// 三个LoadSlot的VM
	LoadScreenModel->InitViewModelForLoadSlot();

	LoadMenuWidget = CreateWidget<ULoadScreenUserWidget>(GetWorld(), LoadMenuWidgetClass);
	LoadMenuWidget->AddToViewport();
	LoadMenuWidget->BlueprintInitializeWidget();

	
	if (APlayerController* FirstPlayerController = GetWorld()->GetFirstPlayerController())
	{
		FInputModeUIOnly InputMode;
		FirstPlayerController->SetInputMode(InputMode);
		FirstPlayerController->bShowMouseCursor = true;
	}
}
