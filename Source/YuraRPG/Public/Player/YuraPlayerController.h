// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "YuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AYuraPlayerController();
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	void Move(const FInputActionValue& InputActionValue);

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> YuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
};
