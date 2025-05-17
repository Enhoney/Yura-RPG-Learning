// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"

FString UYuraGameplayAbility::GetCurrentLevelDescription(int Level)
{
	return FString::Printf(TEXT("<Default>Default Description : %s</>\n<Type>Type : %s</>\n<Level>Level : %d</>"), L"What can I say!!!", L"Offensive", Level);
}

FString UYuraGameplayAbility::GetNextLevelDescription(int Level)
{
	return FString::Printf(TEXT("<Default>Default Description : %s</>\n<Type>Type : %s</>\n<Level>Level : %d</>"), L"Man!!!", L"Offensive", Level);
}

FString UYuraGameplayAbility::GetLockedDescription(int Level)
{
	return FString::Printf(TEXT("<Default>Default Description : %s</>"), L"Mamba out!!!");
}

void UYuraGameplayAbility::SetWarpingTargetFacingFromLocation(const FVector& TargetLocation)
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		CombatInterface->SetWarpTargetFacing(TargetLocation);
	}
}

FTaggedMontage UYuraGameplayAbility::GetRandTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 RandIndex = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[RandIndex];
	}

	return FTaggedMontage();
}