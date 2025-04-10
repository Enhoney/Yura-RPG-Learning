// Copyright Muci Yun.


#include "YuraCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "YuraAbilitySystemComponent.h"
#include "YuraPlayerState.h"

#include "YuraPlayerController.h"
#include "UI/HUD/YuraHUD.h"

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

void AYuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 如果是在服务器（这个函数只会在服务器才会被调用，加上这个是为了保险）
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		InitAbilityActorInfo();
	}
}

void AYuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 因为我们不清楚这个函数会被调用几次，所以加上一个条件
	// 如果是在客户端（主机代理和模拟代理都会复制），并且是第一次
	if (AbilitySystemComponent == nullptr)
	{
		if (GetNetMode() == ENetMode::NM_Client)
		{
			InitAbilityActorInfo();
		}
	}
	
}

void AYuraCharacter::InitAbilityActorInfo()
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();

	check(YuraPlayerState);
	YuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(YuraPlayerState, this);

	AbilitySystemComponent = YuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = YuraPlayerState->GetAttributeSet();

	// 这里不要用断言，因为要知道，在客户端上，多人游戏场景下，其他玩家的Character也是存在的，即便是模拟代理
	// 这个时候，GetController得到的就是空指针，这是很正常的情况
	// 这也就是为什么check要慎用
	if (AYuraPlayerController* YuraPlayerController = Cast<AYuraPlayerController>(GetController()))
	{
		if (AYuraHUD* YuraHUD = Cast<AYuraHUD>(YuraPlayerController->GetHUD()))
		{
			YuraHUD->InitOverlay(YuraPlayerController, YuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
		
	}
}
