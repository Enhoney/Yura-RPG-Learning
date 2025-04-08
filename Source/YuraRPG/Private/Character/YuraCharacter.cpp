// Copyright Muci Yun.


#include "YuraCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

AYuraCharacter::AYuraCharacter()
{
	// 实例化相机肝
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	// 固定旋转
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	// CameraBoom->SetIsReplicated(false);
	// 可以让相机移动延迟
	CameraBoom->bEnableCameraLag = true;

	// 实例化相机
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = false;
	// Camera->SetIsReplicated(false);

	// 设置朝向为移动方向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 设置转向速度
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	/** 下面这几个是俯视角游戏常用的设置*/
	// 将角色的移动限制在特定平面（例如2D平面）。
	GetCharacterMovement()->bConstrainToPlane = true;
	// 在游戏开始时将角色自动对齐到约束平面
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 不要使用Controller的Rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}
