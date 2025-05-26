// Copyright Muci Yun.


#include "YuraCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"
#include "YuraAbilitySystemComponent.h"
#include "YuraPlayerState.h"
#include "Player/Data/LevelUpInfo.h"
#include "YuraPlayerController.h"
#include "UI/HUD/YuraHUD.h"
#include "Interaction/CombatInterface.h"
#include "YuraGameplayTags.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "AbilitySystem/PassiveNiagara/PassiveNiagaraComponent.h"

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

	CharacterClass = ECharacterClass::Elementalist;

	LevelingUpNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelingUpNiagaraComp"));
	LevelingUpNiagaraComp->SetupAttachment(GetRootComponent());
	LevelingUpNiagaraComp->bAutoActivate = false;
	// LevelingUpNiagaraComp->SetRelativeRotation(FRotator(45.f, 0.f, 0.f));

	// 被动技能粒子组件--别忘了在ASC初始初始化完成之后手动调用Init
	HaloOfProtectionNiagara = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("HaloOfProtectionNiagara"));
	HaloOfProtectionNiagara->SetupAttachment(GetRootComponent());
	HealthSiphonNiagara = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("HealthSiphonNiagara"));
	HealthSiphonNiagara->SetupAttachment(GetRootComponent());
	ManaSiphonNiagara = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("ManaSiphonNiagara"));
	ManaSiphonNiagara->SetupAttachment(GetRootComponent());
}

void AYuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 如果是在服务器（这个函数只会在服务器才会被调用，加上这个是为了保险）
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		InitAbilityActorInfo();
		// 注册能力
		AddCharacterAbilities();
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

int32 AYuraCharacter::GetCharacterLevel() const
{
	const AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();
	check(YuraPlayerState);

	return YuraPlayerState->GetCharacterLevel();
}


void AYuraCharacter::AddToExp_Implementation(int32 ExpToAdd)
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();
	check(YuraPlayerState);

	YuraPlayerState->AddToExp(ExpToAdd);
}

void AYuraCharacter::AddToPlayerLevel_Implementation(int32 LevelToAdd)
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();
	check(YuraPlayerState);

	YuraPlayerState->AddToCharacterLevel(LevelToAdd);
	// 判断是否可以解锁新的技能
	UYuraAbilitySystemComponent* YuraASC = Cast<UYuraAbilitySystemComponent>(AbilitySystemComponent);
	check(YuraASC);
	YuraASC->UpdateAbilityStatuses(YuraPlayerState->GetCharacterLevel());
}

void AYuraCharacter::LevelUp_Implementation()
{
	MulticastActivateLevelUpNiagara();
}

int32 AYuraCharacter::GetCurrentExp_Implementation() const
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();
	check(YuraPlayerState);

	return YuraPlayerState->GetPlayerExp();
}

int32 AYuraCharacter::FindCurrentLevelByExp_Implementation(int32 CurExp) const
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();
	check(YuraPlayerState);

	return YuraPlayerState->LevelUpInfo->FindCurrentLevelByExp(CurExp);
}

int32 AYuraCharacter::GetAttributePointReward_Implementation(int32 Level) const
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();
	check(YuraPlayerState);

	return YuraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 AYuraCharacter::GetSpellPointReward_Implementation(int32 Level) const
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();
	check(YuraPlayerState);
	
	return YuraPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void AYuraCharacter::AddAttributePoints_Implementation(int32 AttributePointToAdd)
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();
	check(YuraPlayerState);

	YuraPlayerState->AddToAttributePoint(AttributePointToAdd);
}

void AYuraCharacter::AddSpellPoints_Implementation(int32 SpellPointToAdd)
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();
	check(YuraPlayerState);

	YuraPlayerState->AddToSpellPoint(SpellPointToAdd);
}

int32 AYuraCharacter::GetAttributePoint_Implementation() const
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();
	check(YuraPlayerState);

	return YuraPlayerState->GetAttributePoint();
}

void AYuraCharacter::ConsumeAttributePoint_Implementation(int32 AttributePointToUse)
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();
	check(YuraPlayerState);

	YuraPlayerState->ConsumeAttributePoint(AttributePointToUse);
}

void AYuraCharacter::ConsumeSpellPoint_Implementation(int32 SpellPointToUse)
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();
	check(YuraPlayerState);

	YuraPlayerState->ConsumeSpellPoint(SpellPointToUse);
}


void AYuraCharacter::OnRep_Stunned()
{
	const FYuraGameplayTags YuraTags = FYuraGameplayTags::Get();
	FGameplayTagContainer BlockTags;
	BlockTags.AddTag(YuraTags.Player_Block_CursorTrace);
	BlockTags.AddTag(YuraTags.Player_Block_InputHeld);
	BlockTags.AddTag(YuraTags.Player_Block_InputPressed);
	BlockTags.AddTag(YuraTags.Player_Block_InputReleased);

	// 动态GE不会复制，所以客户端在眩晕的时候，无法禁用这些行为，所以客户端会不断预测和被服务器回溯，导致抽搐
	// 所以我们要在客户端也添加这些Tag
	if (bStunned)
	{
		AbilitySystemComponent->AddLooseGameplayTags(BlockTags);
		StunDebuffNiagaraComp->Activate();
	}
	else
	{
		AbilitySystemComponent->RemoveLooseGameplayTags(BlockTags);
		StunDebuffNiagaraComp->Deactivate();
	}

}

void AYuraCharacter::OnRep_Burned()
{
	if (bStunned)
	{
		BurnDebuffNiagaraComp->Activate();
	}
	else
	{
		BurnDebuffNiagaraComp->Deactivate();
	}
}

void AYuraCharacter::InitAbilityActorInfo()
{
	AYuraPlayerState* YuraPlayerState = GetPlayerState<AYuraPlayerState>();

	check(YuraPlayerState);
	YuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(YuraPlayerState, this);

	AbilitySystemComponent = YuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = YuraPlayerState->GetAttributeSet();

	// 初始化属性
	InitializeDefaultAttributes();

	// 自定义函数，绑定代理
	Cast<UYuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	// 眩晕状态
	AbilitySystemComponent->RegisterGameplayTagEvent(FYuraGameplayTags::Get().Debuff_Lighting_Stun).AddUObject(this, &AYuraCharacter::HandleStunDebuffInAnim);

	// 广播ASC初始化完成--为了绑定代理到DebuffNiagaraComp
	OnASCInitializedDelegate.Broadcast(AbilitySystemComponent);

	// 初始化三个PassiveNiagaraComponent--主要是绑定回调函数
	HaloOfProtectionNiagara->InitPassiveNiagaraComponent(FYuraGameplayTags::Get().Ability_Passive_HaloOfProtection);
	HealthSiphonNiagara->InitPassiveNiagaraComponent(FYuraGameplayTags::Get().Ability_Passive_HealthSiphon);
	ManaSiphonNiagara->InitPassiveNiagaraComponent(FYuraGameplayTags::Get().Ability_Passive_ManaSiphon);

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

void AYuraCharacter::MulticastActivateLevelUpNiagara_Implementation()
{
	if (!IsValid(LevelingUpNiagaraComp)) return;

	// 因为这个游戏视角是固定的，所以所有玩家相机角度都是一样的，不存在出现在屏幕上但是模拟代理上看不到的情况
	const FVector CameraLocation = Camera->GetComponentLocation();
	const FVector NiagaraCompLocation = LevelingUpNiagaraComp->GetComponentLocation();
	// 设置朝向
	LevelingUpNiagaraComp->SetWorldRotation((CameraLocation - NiagaraCompLocation).Rotation());
	LevelingUpNiagaraComp->Activate(true);
}
