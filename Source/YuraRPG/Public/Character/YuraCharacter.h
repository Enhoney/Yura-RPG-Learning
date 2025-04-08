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

private:

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
	
};
