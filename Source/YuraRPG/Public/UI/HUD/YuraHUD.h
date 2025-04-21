// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "YuraHUD.generated.h"

class UYuraUserWidget;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
struct FWidgetControllerParam;


/**
 * 
 */
UCLASS()
class YURARPG_API AYuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION()
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParam& InWCParam);

	UFUNCTION()
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParam& InWCParam);

	// 初始化Overlay并添加到视口
	void InitOverlay(APlayerController* InPC, APlayerState* InPS, UAbilitySystemComponent* InASC, UAttributeSet* InAS);
	

private:
	// 主UI
	UPROPERTY()
	TObjectPtr<UYuraUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UYuraUserWidget> OverlayWidgetClass;
	// overlay widget controller
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	// Attribute Menu Widget Controller
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuController;
	
};
