// Copyright Muci Yun.


#include "YuraEnemy.h"
#include "Components/CapsuleComponent.h"
#include "YuraRPG.h"

#include "YuraAbilitySystemComponent.h"
#include "YuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/YuraUserWidget.h"

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
}

void AYuraEnemy::HighlightActor()
{
	// 开启高亮（这个设置和材质有关，不必深究）
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_FOR_HIGHLIGHT_RED);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_FOR_HIGHLIGHT_RED);
}

void AYuraEnemy::UnhighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AYuraEnemy::GetCharacterLevel()
{
	return Level;
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

void AYuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

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

	
}

void AYuraEnemy::InitAbilityActorInfo()
{
	// 初始化
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// 自定义函数，绑定代理
	Cast<UYuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}
