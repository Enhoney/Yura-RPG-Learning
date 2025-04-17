// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Character/YuraCharacterBase.h"
#include "EnemyInterface.h"
#include "Interaction/CombatInterface.h"
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

	/** Enemy Interface start*/
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
	/** Enemy Interface end*/

	/** Combat Interface start*/
	virtual int32 GetCharacterLevel() override;
	/** Combat Interface end*/

protected:
	void BeginPlay() override;

private:

	virtual void InitAbilityActorInfo() override;
public:
	UPROPERTY(BlueprintReadOnly)
	bool bHighlight = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Default Class")
	int32 Level = 1;
	
};
