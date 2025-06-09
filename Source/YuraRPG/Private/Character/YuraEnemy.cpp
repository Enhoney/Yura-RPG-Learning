// Copyright Muci Yun.


#include "YuraEnemy.h"
#include "Components/CapsuleComponent.h"
#include "YuraRPG.h"

#include "YuraAbilitySystemComponent.h"
#include "YuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/YuraUserWidget.h"
#include "YuraAbilitySystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "YuraGameplayTags.h"

#include "AI/YuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AYuraEnemy::AYuraEnemy()
{
	// 实例化ASC
	AbilitySystemComponent = CreateDefaultSubobject<UYuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// 实例化AS
	AttributeSet = CreateDefaultSubobject<UYuraAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);

	// 转向设置
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
}

void AYuraEnemy::HighlightActor_Implementation()
{
	// 开启高亮（这个设置和材质有关，不必深究）
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_FOR_HIGHLIGHT_RED);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_FOR_HIGHLIGHT_RED);
}

void AYuraEnemy::UnhighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AYuraEnemy::GetCharacterLevel() const
{
	return CharacterLevel;
}

void AYuraEnemy::Die(const FVector& InDeathImpulse)
{
	if (HasAuthority())
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);
	}
	
	// 设置寿命
	SetLifeSpan(LifeSpawnOnDeath);
	// 
	Super::Die(InDeathImpulse);
}

AActor* AYuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AYuraEnemy::SetCombatTarget_Implementation(AActor* TargetActor)
{
	CombatTarget = TargetActor;
}

void AYuraEnemy::CallInitHealthValue()
{
	// Init
	if (const UYuraAttributeSet* YuraAttributeSet = Cast<UYuraAttributeSet>(AttributeSet))
	{
		OnHealthChanged.Broadcast(YuraAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(YuraAttributeSet->GetMaxHealth());
	}
}

void AYuraEnemy::OnHitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = (NewCount > 0);

	GetCharacterMovement()->MaxWalkSpeed = (bHitReacting ? 0.f : BaseWalkSpeed);

	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsHitReacting"), bHitReacting);
}

void AYuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 双保险，虽然这个函数只会在服务器上调用
	if (!HasAuthority()) return;

	EnemyAIController = Cast<AYuraAIController>(NewController);

	// 将行为树和黑板进行绑定--使用在行为树资产中设置的那个黑板资产进行绑定
	EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*EnemyBehaviorTree->BlackboardAsset);
	// 运行行为树
	EnemyAIController->RunBehaviorTree(EnemyBehaviorTree);
	// 初始化受击状态
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsHitReacting"), false);

	// 初始化眩晕状态
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsStunned"), false);

	// 是否为远程攻击角色
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsRangedAttacker"), (CharacterClass != ECharacterClass::Warrior));

	// 设置是否死亡
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), false);
}

void AYuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	// 授予通用能力和独有能力
	if (HasAuthority())
	{
		UYuraAbilitySystemLibrary::GrantStartUpAbilities(this, AbilitySystemComponent, CharacterClass, CharacterLevel);
	}
	
	// 设置WidgetController
	if (UYuraUserWidget* YuraUI = Cast<UYuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		YuraUI->SetWidgetController(this);
	}

	if (const UYuraAttributeSet* YuraAttributeSet = Cast<UYuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(YuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnHealthChanged.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(YuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
		
	}

	CallInitHealthValue();

	// 绑定Tag授予回调--HitReact
	if (HasAuthority())
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(FYuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &AYuraEnemy::OnHitReactTagChange
		);

		// 眩晕状态回调
		AbilitySystemComponent->RegisterGameplayTagEvent(FYuraGameplayTags::Get().Debuff_Lighting_Stun).AddUObject(this, &AYuraEnemy::HandleStunDebuffInAnim);
	}
}

void AYuraEnemy::InitializeDefaultAttributes() const
{
	UYuraAbilitySystemLibrary::InitDefaultAttributes(this, CharacterClass, CharacterLevel, AbilitySystemComponent);
}

void AYuraEnemy::HandleStunDebuffInAnim(const FGameplayTag InDebuffTag, int32 NewCount)
{
	bStunned = (NewCount > 0);

	GetCharacterMovement()->MaxWalkSpeed = (bStunned ? 0.f : BaseWalkSpeed);

	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsStunned"), bStunned);
}

void AYuraEnemy::InitAbilityActorInfo()
{
	// 初始化
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// 自定义函数，绑定代理
	Cast<UYuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	// 初始化属性
	InitializeDefaultAttributes();

	// 广播ASC初始化完成--为了绑定代理到DebuffNiagaraComp
	OnASCInitializedDelegate.Broadcast(AbilitySystemComponent);
}
