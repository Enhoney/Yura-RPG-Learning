// Copyright Muci Yun.


#include "YuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"

AYuraPlayerController::AYuraPlayerController()
{
	// 开启网络复制
	bReplicates = true;
}

void AYuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(YuraContext);
	// 获取增强输入子系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(YuraContext, 0);

	// 显示鼠标光标并设置光标样式
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// 设置游戏输入模式为GameAndUI
	FInputModeGameAndUI InputModeData;
	// 不要将鼠标锁定在视口--允许我们点击
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// 是否在鼠标按下导致的临时鼠标捕获期间隐藏光标--不隐藏
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);


}

void AYuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AYuraPlayerController::Move);
}

void AYuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// 将输入转换成2D形式
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	// 只使用偏航角
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const FVector ForawrdDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForawrdDirection, InputAxisVector.X);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.Y);
	}
}
