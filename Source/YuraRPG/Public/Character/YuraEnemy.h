// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Character/YuraCharacterBase.h"
#include "EnemyInterface.h"
#include "YuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraEnemy : public AYuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:

	AYuraEnemy();

	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;

protected:
	void BeginPlay() override;


public:
	UPROPERTY(BlueprintReadOnly)
	bool bHighlight = false;
	
};
