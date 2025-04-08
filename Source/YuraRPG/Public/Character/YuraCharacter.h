// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Character/YuraCharacterBase.h"
#include "YuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraCharacter : public AYuraCharacterBase
{
	GENERATED_BODY()

public:
	AYuraCharacter();

	// 被Controlelr掌控的时候--服务端在这里初始化ASC
	virtual void PossessedBy(AController* NewController) override;

	// PlayerState网络复制过来的时候
	virtual void OnRep_PlayerState() override;

private:

	void InitAbilityActorInfo();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
	
};
