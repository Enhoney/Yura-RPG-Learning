// Copyright Muci Yun.


#include "AbilitySystem/PassiveNiagara/PassiveNiagaraComponent.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/YuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UPassiveNiagaraComponent::InitPassiveNiagaraComponent(const FGameplayTag& InPassivaAbilityTag)
{
	PassiveAbilityTag = InPassivaAbilityTag;

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		UYuraAbilitySystemComponent* YuraASC = Cast<UYuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()));

		if (YuraASC)
		{
			YuraASC->OnPassiveAbilityEquipAndUnload.AddUObject(this, &UPassiveNiagaraComponent::HandlePassiveNiagara);
		}

		CombatInterface->GetOnActorDeathDelegate().AddDynamic(this, &UPassiveNiagaraComponent::OnOwnerDeath);
	}
}

void UPassiveNiagaraComponent::HandlePassiveNiagara(const FGameplayTag& InPassiveAbilityTag, bool bInIsEquipped)
{
	if (InPassiveAbilityTag.MatchesTagExact(PassiveAbilityTag))
	{
		if (bInIsEquipped)
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}

void UPassiveNiagaraComponent::OnOwnerDeath(AActor* OwnerActor)
{
	Deactivate();
}
