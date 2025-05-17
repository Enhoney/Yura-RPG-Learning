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

	YuraASC->OnAbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& NewStatusTag, int32 NewAbilityLevel)
		{
			check(AbilityInformations);
			FYuraAbilityInfo AbilityInfo = AbilityInformations->FindAbilityInfoByTag(AbilityTag);
			AbilityInfo.AbilityStatusTag = NewStatusTag;
			AbilityInfo.AbilityLevel = NewAbilityLevel;
			OnAbilityInfoGivenDelegate.Broadcast(AbilityInfo);
			// 处理按钮状态
			if (AbilityTag.MatchesTagExact(SelectedAbility.AbilityTag))
			{
				HandleButtonEnableOnChanged(CurrentSpellPoint, NewStatusTag);
			}
		});

	// 更新技能点
	AYuraPlayerState* YuraPlayerState = CastChecked<AYuraPlayerState>(PlayerState);
	// 广播技能点
	YuraPlayerState->OnSpellPointChangedDelegate.AddLambda([this](int32 NewSpellPoints)
		{
			OnSpellPointChangedDelegate.Broadcast(NewSpellPoints);
			// 处理按钮状态
			HandleButtonEnableOnChanged(NewSpellPoints, SelectedAbility.StatusTag);
		});


}

void USpellMenuWidgetController::HandleSpellPointAndEquipButton(const FGameplayTag& InAbilityTag)
{
	UYuraAbilitySystemComponent* ASC = CastChecked<UYuraAbilitySystemComponent>(AbilitySystemComponent);

	FGameplayTag AbilityStatusTag = ASC->GetStatusByAbilityTag(InAbilityTag);

	// 赋值SelectedAbility
	SelectedAbility.AbilityTag = InAbilityTag;
	SelectedAbility.StatusTag = AbilityStatusTag;

	// 默认都是不可用
	bool bCanSpellPoint = false;
	bool bCanEquip = false;

	// 技能未赋予--锁定状态
	if (!AbilityStatusTag.IsValid())
	{
		// 获取描述信息
		FString CurDescription = FString();
		FString NextLevelDescription = FString();
		UYuraAbilitySystemComponent* YuraASC = CastChecked<UYuraAbilitySystemComponent>(AbilitySystemComponent);
		YuraASC->GetAbilityCurrentDescription(SelectedAbility.AbilityTag, CurDescription, NextLevelDescription);

		OnSpellMenuButtonDelegate.Broadcast(bCanSpellPoint, bCanEquip, CurDescription, NextLevelDescription);
		return;
	}

	AYuraPlayerState* YuraPlayerState = CastChecked<AYuraPlayerState>(PlayerState);
	CurrentSpellPoint = YuraPlayerState->GetSpellPoint();
	// 如果是可解锁状态
	if (AbilityStatusTag.MatchesTagExact(FYuraGameplayTags::Get().Ability_Status_Eligible))
	{
		// TODO : 不同技能解锁和升级需要的技能点应该是不同的，后续设计，这里默认只用1点
		if (CurrentSpellPoint > 0)
		{
			bCanSpellPoint = true;
		}
	}
	else if (AbilityStatusTag.MatchesTagExact(FYuraGameplayTags::Get().Ability_Status_Unlocked) ||
		AbilityStatusTag.MatchesTagExact(FYuraGameplayTags::Get().Ability_Status_Equipped))
	{
		// 解锁和装备状态
		bCanEquip = true;
		if (CurrentSpellPoint > 0)
		{
			bCanSpellPoint = true;
		}
	}

	// 获取描述信息
	FString CurDescription = FString();
	FString NextLevelDescription = FString();
	UYuraAbilitySystemComponent* YuraASC = CastChecked<UYuraAbilitySystemComponent>(AbilitySystemComponent);
	YuraASC->GetAbilityCurrentDescription(SelectedAbility.AbilityTag, CurDescription, NextLevelDescription);

	OnSpellMenuButtonDelegate.Broadcast(bCanSpellPoint, bCanEquip, CurDescription, NextLevelDescription);
}


void USpellMenuWidgetController::ClearSelectedAbility()
{
	SelectedAbility.AbilityTag = FGameplayTag();
	SelectedAbility.StatusTag = FGameplayTag();

	CurrentSpellPoint = 0;
}

void USpellMenuWidgetController::OnSpellPointButtonPressed()
{
	UYuraAbilitySystemComponent* ASC = CastChecked<UYuraAbilitySystemComponent>(AbilitySystemComponent);

	// 执行RPC
	ASC->ServerSpendignSpellPoint(SelectedAbility.AbilityTag, 1);
}

void USpellMenuWidgetController::HandleButtonEnableOnChanged(int32 NewSpellPoint, const FGameplayTag& NewStatusTag)
{
	// 默认都是不可用
	bool bCanSpellPoint = false;
	bool bCanEquip = false;

	// 获取当前的技能点
	AYuraPlayerState* YuraPlayerState = CastChecked<AYuraPlayerState>(PlayerState);
	const int32 SpellPoint = YuraPlayerState->GetSpellPoint();

	// 如果此时没有选中技能
	if (!SelectedAbility.AbilityTag.IsValid())
	{
		// 啥也不干，都不需要执行广播
		return;
	}

	// 更新状态
	SelectedAbility.StatusTag = NewStatusTag;
	CurrentSpellPoint = NewSpellPoint;

	// TODO : 根据技能需要的点数进行判断
	if (CurrentSpellPoint > 0)
	{
		// 技能点大于0，只需要确定是否可解锁或者升级
		if (SelectedAbility.StatusTag.MatchesTagExact((FYuraGameplayTags::Get().Ability_Status_Eligible)) ||
			SelectedAbility.StatusTag.MatchesTagExact((FYuraGameplayTags::Get().Ability_Status_Unlocked)) ||
			SelectedAbility.StatusTag.MatchesTagExact((FYuraGameplayTags::Get().Ability_Status_Equipped)))
		{
			bCanSpellPoint = true;
		}
	}

	// 无论技能点是否为0，根据状态确定是否可装备
	if (SelectedAbility.StatusTag.MatchesTagExact((FYuraGameplayTags::Get().Ability_Status_Unlocked)) ||
		SelectedAbility.StatusTag.MatchesTagExact((FYuraGameplayTags::Get().Ability_Status_Equipped)))
	{
		bCanEquip = true;
	}

	// 获取描述信息
	FString CurDescription = FString();
	FString NextLevelDescription = FString();
	UYuraAbilitySystemComponent* YuraASC = CastChecked<UYuraAbilitySystemComponent>(AbilitySystemComponent);
	YuraASC->GetAbilityCurrentDescription(SelectedAbility.AbilityTag, CurDescription, NextLevelDescription);

	OnSpellMenuButtonDelegate.Broadcast(bCanSpellPoint, bCanEquip, CurDescription, NextLevelDescription);
}
