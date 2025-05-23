// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraBeamSpell.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

void UYuraBeamSpell::StoreCursorHitInfo(const FHitResult& CursorHitResult)
{
	if (CursorHitResult.bBlockingHit)
	{
		CursorHitLocation = CursorHitResult.Location;

		CursorHitActor = CursorHitResult.GetActor();
	}
	else
	{
		// 如果没有命中目标，就直接取消这个技能，并复制
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
	
}

void UYuraBeamSpell::StoreOwnerOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerPlayerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	}
}


