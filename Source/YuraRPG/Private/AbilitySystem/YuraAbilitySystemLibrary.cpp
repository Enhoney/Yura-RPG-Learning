// Copyright Muci Yun.


#include "AbilitySystem/YuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/YuraHUD.h"
#include "UI/WidgetController/YuraWidgetController.h"
#include "YuraPlayerState.h"

UOverlayWidgetController* UYuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* InWorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(InWorldContextObject, 0))
	{
		if (AYuraHUD* YuraHUD = Cast<AYuraHUD>(PC->GetHUD()))
		{
			AYuraPlayerState* PS = PC->GetPlayerState<AYuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParam WidgetControllerParam(PC, PS, ASC, AS);

			return YuraHUD->GetOverlayWidgetController(WidgetControllerParam);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UYuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* InWorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(InWorldContextObject, 0))
	{
		if (AYuraHUD* YuraHUD = Cast<AYuraHUD>(PC->GetHUD()))
		{
			AYuraPlayerState* PS = PC->GetPlayerState<AYuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParam WidgetControllerParam(PC, PS, ASC, AS);

			return YuraHUD->GetAttributeMenuWidgetController(WidgetControllerParam);
		}
	}
	return nullptr;
}
