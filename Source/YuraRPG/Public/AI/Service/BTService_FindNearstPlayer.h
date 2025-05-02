// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindNearstPlayer.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UBTService_FindNearstPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()


protected:
	// Tick
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector TargetToFollowKeySelector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector DistanceToTargetKeySelector;
};
