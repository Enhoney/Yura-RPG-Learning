// Copyright Muci Yun.


#include "YuraPlayerState.h"

#include "YuraAbilitySystemComponent.h"
#include "YuraAttributeSet.h"

#include "Net/UnrealNetwork.h"


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

void AYuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(AYuraPlayerState, Level, COND_None, REPNOTIFY_Always);
}

UAbilitySystemComponent* AYuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AYuraPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}


void AYuraPlayerState::OnRep_Level(int32 OldLevel)
{
}
