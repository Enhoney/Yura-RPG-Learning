// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraDamageGameplayAbility.h"

#include "Interaction/CombatInterface.h"
void UYuraDamageGameplayAbility::SetWarpingTargetFacingFromLocation(const FVector& TargetLocation)
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		CombatInterface->SetWarpTargetFacing(TargetLocation);
	}
}