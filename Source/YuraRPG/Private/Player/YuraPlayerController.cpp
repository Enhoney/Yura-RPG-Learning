// Copyright Muci Yun.


#include "YuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "Input/YuraInputComponent.h"
#include "HighlightInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "YuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "YuraGameplayTags.h"

// 导航系统
#include "NavigationSystem.h"
#include "NavigationPath.h"

#include "GameFramework/Character.h"
#include "UI/Widget/DamageFloatingComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Actor/MagicCircle.h"
#include "../YuraRPG.h"

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

	// 自动更新Magic位置
	UpdateMacicCircleLocation();

	YuraAutoRunning();
}

void AYuraPlayerController::ShowDamageText_Implementation(float DamageFloating, ACharacter* TargetCharacter, bool bDamageBlock, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageFloatingCompClass)
	{
		// 只在主机代理上做
		if (IsLocalController())
		{
			// 创建组件
			UDamageFloatingComponent* DamageFloatingComp = NewObject<UDamageFloatingComponent>(TargetCharacter, DamageFloatingCompClass);
			// 注册组件，动态创建组件需要这么做才行
			DamageFloatingComp->RegisterComponent();
			// 附着一下只是为了开始播放UI动画，之后还是要分离的
			DamageFloatingComp->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			DamageFloatingComp->SetDamageFloatingText(DamageFloating, bDamageBlock, bCriticalHit);
			// 分离，让这个数字不跟随敌人移动而移动
			DamageFloatingComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		}
	}
	
	
}

void AYuraPlayerController::ShowMagicCircle()
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
	}
	
}

void AYuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void AYuraPlayerController::SetMagicCircleMaterial(UMaterialInterface* InMagicCircleMat)
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetMagicCircleMaterial(InMagicCircleMat);
	}
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
	// Shift 
	YuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AYuraPlayerController::ShiftPressed);
	YuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AYuraPlayerController::ShiftReleased);

	YuraInputComponent->BindAbilityActions(AbilityInputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AYuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// 如果禁止输入按下，也不能移动
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FYuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}

	// 退出自动移动状态
	bAutoRunning = false;
	FollowingTime = 0.f;

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

void AYuraPlayerController::ShiftPressed()
{
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FYuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}

	bShiftDown = true;
}

void AYuraPlayerController::ShiftReleased()
{
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FYuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}

	bShiftDown = false;
}

void AYuraPlayerController::CursorTrace()
{
	// 如果禁用了光标追踪就取消现在高亮的这些Actor
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FYuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastActor)
		{
			IHighlightInterface::Execute_UnhighlightActor(LastActor);
		}
		if (ThisActor)
		{
			IHighlightInterface::Execute_UnhighlightActor(ThisActor);
		}
		LastActor = nullptr;
		ThisActor = nullptr;

		return;
	}

	// 在执行MagicCircle的时候，忽略Character
	ECollisionChannel CursorTraceChannel = IsValid(MagicCircle) ? ECC_ExceptCharacter : ECC_Visibility;
	GetHitResultUnderCursor(CursorTraceChannel, false, CursorHit);

	// 如果没有命中目标
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastActor = ThisActor;

	// 只有在命中目标实现了接口时才赋值
	if (CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = CursorHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}
	
	if (LastActor != ThisActor)
	{
		if (LastActor)
		{
			IHighlightInterface::Execute_UnhighlightActor(LastActor);
		}
		if (ThisActor)
		{
			IHighlightInterface::Execute_HighlightActor(ThisActor);
		}
	}

}

void AYuraPlayerController::AbilityInputTagPressed(FGameplayTag AbilityActionTag)
{
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FYuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}

	// 只针对鼠标左键输入响应
	if (AbilityActionTag.MatchesTagExact(FYuraGameplayTags::Get().InputTag_LMB))
	{

		if (!IsValid(ThisActor))
		{
			// 如果它连HithlightInterface都没实现，那就是没有NoTargeting
			TargetingStatus = ETargetingStatus::NoTargeting;
		}
		else
		{
			if (ThisActor->ActorHasTag(FName("YuraCharacter.Enemy")))
			{
				TargetingStatus = ETargetingStatus::TargetingEnemy;
			}
			else
			{
				TargetingStatus = ETargetingStatus::TragetingMapEntrance;
			}
		}
		bAutoRunning = false;	// 默认值
	}

	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->AbilityInputTagPressed(AbilityActionTag);
	}
	
}

void AYuraPlayerController::AbilityInputTagReleased(FGameplayTag AbilityActionTag)
{
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FYuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}

	if (GetAbilitySystemComponent() == nullptr) 
	{
		return;
	}
	// 如果不是鼠标左键，就激活能力
	if (!AbilityActionTag.MatchesTagExact(FYuraGameplayTags::Get().InputTag_LMB))
	{
		GetAbilitySystemComponent()->AbilityInputTagReleased(AbilityActionTag);
		return;
	}

	GetAbilitySystemComponent()->AbilityInputTagReleased(AbilityActionTag);
	if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftDown)
	{
		APawn* ControlledPawn = GetPawn<APawn>();
		// 这就表示短按，这个时候我们要去创建一条路径，这需要导航系统了
		if (FollowingTime <= ShortPressThreshould && ControlledPawn)
		{
			if (TargetingStatus == ETargetingStatus::TragetingMapEntrance && IsValid(ThisActor))
			{
				// 短按，移动到HighlightActor的指定位置
				IHighlightInterface::Execute_SetMoveToDestination(ThisActor, CachedDestinationLocation);
			}

			if (TargetingStatus == ETargetingStatus::NoTargeting)
			{
				if (AbilitySystemComponent && !AbilitySystemComponent->HasMatchingGameplayTag(FYuraGameplayTags::Get().Player_Block_InputPressed))
				{
					// 在这个位置放一个箭头
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestinationLocation);
				}
			}

			const FVector CurrentLocation = ControlledPawn->GetActorLocation();
			// 生成一条导航路径
			if (UNavigationPath* NavMovePath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, CurrentLocation, CachedDestinationLocation))
			{
				// 清除组件中原有的曲线路径点
				SplineComponent->ClearSplinePoints();

				for (const FVector& PathPoint : NavMovePath->PathPoints)
				{
					// 添加到Spline组件中去
					SplineComponent->AddSplineWorldPoint(PathPoint);
				}

				// 为了处理某些到不了的位置，直接将导航路径最后一个点作为目标位置
				if (NavMovePath->PathPoints.Num() > 0) 
				{
					CachedDestinationLocation = NavMovePath->PathPoints[NavMovePath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			
			}

		}
		// 重置FollowingTime
		FollowingTime = 0.f;
		TargetingStatus = ETargetingStatus::NoTargeting;
	}
}

void AYuraPlayerController::AbilityInputTagHeld(FGameplayTag AbilityActionTag)
{
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FYuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}

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
	// 或者这个时候按下了鼠标左键
	if (TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftDown)
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
	if (CursorHit.bBlockingHit)
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

void AYuraPlayerController::YuraAutoRunning()
{
	if (bAutoRunning)
	{
		if (APawn* ControlledPawn = GetPawn<APawn>())
		{

			// 找到最接近的一个路径点位置--世界坐标这个是会根据之前添加的点，绘制一条曲线出来，然后从那个曲线上面取点，不仅限于添加的那几个路径点
			const FVector ClosetPoint = SplineComponent->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
			// 计算移动朝向
			const FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
			// 移动
			ControlledPawn->AddMovementInput(Direction, 1.f);

			// 检查距离阈值
			const float DistanceToDestination = (ControlledPawn->GetActorLocation() - CachedDestinationLocation).Length();
			if (DistanceToDestination <= AutoRunAcceptenceRadius)
			{
				// 如果离点击的位置已经少于50cm，就认为已经到达了
				bAutoRunning = false;
			}
		}
	}
}

void AYuraPlayerController::UpdateMacicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}
