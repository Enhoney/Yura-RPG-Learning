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

	DOREPLIFETIME_CONDITION_NOTIFY(AYuraPlayerState, Exp, COND_None, REPNOTIFY_Always);
}

UAbilitySystemComponent* AYuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AYuraPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void AYuraPlayerState::SetCharacterLevel(int32 NewLevel)
{
	Level = FMath::Max<int32>(0, NewLevel);

	OnPlayerLevelChangedDelegate.Broadcast(Level);
}

void AYuraPlayerState::AddToCharacterLevel(int32 LevelToAdd)
{
	Level = FMath::Max<int32>(0, Level + LevelToAdd);

	OnPlayerLevelChangedDelegate.Broadcast(Level);
}


void AYuraPlayerState::SetExp(int32 NewExp)
{
	Exp = FMath::Max<int32>(0, NewExp);

	OnPlayerExpChangedDelegate.Broadcast(Exp);
}

void AYuraPlayerState::AddToExp(int32 ExpToAdd)
{
	Exp = FMath::Max<int32>(0, Exp + ExpToAdd);

	OnPlayerExpChangedDelegate.Broadcast(Exp);
}

void AYuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnPlayerLevelChangedDelegate.Broadcast(Level);
}

void AYuraPlayerState::OnRep_Exp(int32 OldExp)
{
	OnPlayerExpChangedDelegate.Broadcast(Exp);
}
