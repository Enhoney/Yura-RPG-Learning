// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "YuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

class IEnemyInterface;

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

	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	void Move(const FInputActionValue& InputActionValue);

	// 执行调用敌人高亮逻辑
	void CursorTrace();

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> YuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	// 记录上一帧和这一帧命中的对象
	TObjectPtr<IEnemyInterface> LastActor =  nullptr;
	TObjectPtr<IEnemyInterface> ThisActor = nullptr;
};
