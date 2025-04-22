// Copyright Muci Yun.


#include "YuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "Input/YuraInputComponent.h"
#include "EnemyInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "YuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "YuraGameplayTags.h"

AYuraPlayerController::AYuraPlayerController()
{

	// 开启tick
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

}

void AYuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AYuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(YuraContext);
	// 获取增强输入子系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) 
	{
		Subsystem->AddMappingContext(YuraContext, 0);
	}

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

	UYuraInputComponent* YuraInputComponent = CastChecked<UYuraInputComponent>(InputComponent);

	YuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AYuraPlayerController::Move);

	YuraInputComponent->BindAbilityActions(AbilityInputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
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

void AYuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;

	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	// 如果没有命中目标
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastActor = ThisActor;

	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/** 
	 * 下面是几个场景
	 * 1、两个都是空指针--啥也不干
	 * 2、LastActor是空指针，ThisActor不是--有命中的对象了--开启高亮
	 * 3、LastActor不是空指针，ThisActor是空指针--关闭高亮
	 * 4、两个都不是空指针，并且二者相同--啥也不干--维持高亮
	 * 5、两个都不是空指针，但是两个对象不一样了--关闭上一个的高亮，开启这个的高亮
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case 2
			ThisActor->HighlightActor();
		}
		// Case 1
	}
	else
	{
		if (ThisActor == nullptr)
		{
			// Case 3
			LastActor->UnhighlightActor();
		}
		else
		{
			if (ThisActor != LastActor)
			{
				// Case 5
				LastActor->UnhighlightActor();
				ThisActor->HighlightActor();
			}
			// Case 4
		}
	}

}

void AYuraPlayerController::AbilityInputTagPressed(FGameplayTag AbilityActionTag)
{
	// 只针对鼠标左键输入响应
	if (AbilityActionTag.MatchesTagExact(FYuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = (ThisActor == nullptr) ? false : true;
		bAutoRunning = false;	// 默认值
	}
	
}

void AYuraPlayerController::AbilityInputTagReleased(FGameplayTag AbilityActionTag)
{
	if (GetAbilitySystemComponent() == nullptr) 
	{
		return;
	}
	GetAbilitySystemComponent()->AbilityInputTagReleased(AbilityActionTag);
}

void AYuraPlayerController::AbilityInputTagHeld(FGameplayTag AbilityActionTag)
{
	if (GetAbilitySystemComponent() == nullptr)
	{
		return;
	}
	// 如果不是鼠标左键，就激活能力
	if (!AbilityActionTag.MatchesTagExact(FYuraGameplayTags::Get().InputTag_LMB))
	{
		GetAbilitySystemComponent()->AbilityInputTagHeld(AbilityActionTag);
		return;
	}
	// 如果锁定了目标--鼠标下方是敌人，并且按住了鼠标左键
	if (bTargeting)
	{
		// 这对吗，这对应的是什么能力--通常就是攻击能力
		GetAbilitySystemComponent()->AbilityInputTagHeld(AbilityActionTag);
		return;
	}
	else
	{
		// 这个时候就是点击移动了，不用激活GA
		// 增加按下时间
		FollowingTime += GetWorld()->DeltaTimeSeconds;
	}
	// 获取点击位置，因为是可见通道只要点击的位置下有可见物体，就一定会返回true
	FHitResult CursorHit;
	if (GetHitResultUnderCursor(ECC_Visibility, false, CursorHit))
	{
		// 缓存点击位置
		CachedDestinationLocation = CursorHit.ImpactPoint;
	}
	
	// 开始移动
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// 计算移动朝向
		FVector WorldDirection = (CachedDestinationLocation - ControlledPawn->GetActorLocation()).GetSafeNormal();

		ControlledPawn->AddMovementInput(WorldDirection, 1.0f);
	}
	
}

UYuraAbilitySystemComponent* AYuraPlayerController::GetAbilitySystemComponent()
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = 
			Cast<UYuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AbilitySystemComponent;
}
