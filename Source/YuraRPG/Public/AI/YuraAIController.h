// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "YuraAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraAIController : public AAIController
{
	GENERATED_BODY()

public:
	AYuraAIController();

	FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComponent() { return BehaviorTreeComponent; }

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
};
