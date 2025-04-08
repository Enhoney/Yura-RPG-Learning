// Copyright Muci Yun.


#include "YuraPlayerState.h"

#include "YuraAbilitySystemComponent.h"
#include "YuraAttributeSet.h"

AYuraPlayerState::AYuraPlayerState()
{
	NetUpdateFrequency = 100.f;

	// 实例化ASC
	AbilitySystemComponent = CreateDefaultSubobject<UYuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 实例化AS
	AttributeSet = CreateDefaultSubobject<UYuraAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AYuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AYuraPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
