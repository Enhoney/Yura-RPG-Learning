// Copyright Muci Yun.


#include "AbilitySystem/YuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/YuraHUD.h"
#include "UI/WidgetController/YuraWidgetController.h"
#include "YuraPlayerState.h"
#include "Game/YuraGameModeBase.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"

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

void UYuraAbilitySystemLibrary::InitDefaultAttributes(const UObject* InWorldContextObject, ECharacterClass EnemyClass, float CharacterLevel, UAbilitySystemComponent* TargetASC)
{
	AYuraGameModeBase* YuraGameMode = Cast<AYuraGameModeBase>(UGameplayStatics::GetGameMode(InWorldContextObject));

	// 因为在客户端是拿不到GameMode的
	if (YuraGameMode == nullptr)
	{
		return;
	}

	// 用于设置SourceObject
	const AActor* AvatorActor = TargetASC->GetAvatarActor();

	// 获取CharacterInfo
	UCharacterClassInfo* CharacterInfo = YuraGameMode->DefaultEnemyInfo;

	const FCharacterClassDefaultInfo DefaultCharacterInfo = CharacterInfo->GetDerfaultCharacterInfo(EnemyClass);
	// 初始化Primary Attribute
	FGameplayEffectContextHandle PrimaryAttributeContextHandle = TargetASC->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(AvatorActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = TargetASC->MakeOutgoingSpec(DefaultCharacterInfo.DefaultPrimaryAttribute, CharacterLevel, PrimaryAttributeContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());
	// 初始化次要属性
	FGameplayEffectContextHandle SecondaryAttributeContextHandle = TargetASC->MakeEffectContext();
	SecondaryAttributeContextHandle.AddSourceObject(AvatorActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = TargetASC->MakeOutgoingSpec(CharacterInfo->DefaultSecondaryAttribute, CharacterLevel, SecondaryAttributeContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());
	// 初始化重要属性
	FGameplayEffectContextHandle VitalAttributeContextHandle = TargetASC->MakeEffectContext();
	VitalAttributeContextHandle.AddSourceObject(AvatorActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = TargetASC->MakeOutgoingSpec(CharacterInfo->DefaultVitalAttribute, CharacterLevel, VitalAttributeContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}
