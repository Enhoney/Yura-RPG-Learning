// Copyright Muci Yun.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/YuraAbilitySystemComponent.h"
#include "Data/AbilityInfo.h"
#include "Player/YuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastYuraAbilityInfo();

	// 广播初始技能点
	const AYuraPlayerState* YuraPlayerState = CastChecked<AYuraPlayerState>(PlayerState);
	OnSpellPointChangedDelegate.Broadcast(YuraPlayerState->GetSpellPoint());
}

void USpellMenuWidgetController::BindCallbacksToDependiencies()
{
	UYuraAbilitySystemComponent* YuraASC = CastChecked<UYuraAbilitySystemComponent>(AbilitySystemComponent);

	YuraASC->OnAbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& NewStatusTag)
		{
			check(AbilityInformations);
			FYuraAbilityInfo AbilityInfo = AbilityInformations->FindAbilityInfoByTag(AbilityTag);
			AbilityInfo.AbilityStatusTag = NewStatusTag;
			OnAbilityInfoGivenDelegate.Broadcast(AbilityInfo);
		});

	// 更新技能点
	AYuraPlayerState* YuraPlayerState = CastChecked<AYuraPlayerState>(PlayerState);
	// 广播技能点
	YuraPlayerState->OnSpellPointChangedDelegate.AddLambda([this](int32 NewSpellPoints)
		{
			OnSpellPointChangedDelegate.Broadcast(NewSpellPoints);
		});


}
