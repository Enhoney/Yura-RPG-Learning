// Copyright Muci Yun.


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Interaction/CombatInterface.h"

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCInitializedDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
			{
				InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved)
					.AddUObject(this, &UDebuffNiagaraComponent::HandleDebuffNiagara);
			});

		CombatInterface->GetOnActorDeathDelegate().AddDynamic(this, &UDebuffNiagaraComponent::OnOwnerDeath);
	}

}

void UDebuffNiagaraComponent::HandleDebuffNiagara(const FGameplayTag InDebuffTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		Activate();
	}
	else if (NewCount == 0)
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* OwnerActor)
{
	Deactivate();
}
