// Copyright Muci Yun.


#include "Interaction/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.

int32 ICombatInterface::GetCharacterLevel() const
{
	return 0;
}


void ICombatInterface::SetWarpTargetFacing(const FVector& TargetLocation)
{
}

ECharacterClass ICombatInterface::GetCharacterClass() const
{
	return ECharacterClass();
}

