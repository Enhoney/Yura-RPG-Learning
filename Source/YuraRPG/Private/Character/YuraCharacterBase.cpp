// Copyright Muci Yun.


#include "YuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

#include "YuraAbilitySystemComponent.h"

AYuraCharacterBase::AYuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

UAbilitySystemComponent* AYuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AYuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void AYuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AYuraCharacterBase::InitializeDefaultAttributes() const
{
	// 初始化PrimaryAttributes
	ApplyGameplayEffectToSelf(DefaultPrimaryAttributes);
	// 初始化SecondaryAttributes，这个必须在初始化完PrimaryAttribute之后
	ApplyGameplayEffectToSelf(DefaultSedcondaryAttributes);
	// 初始化生命值和魔法值，这个必须在MaxHealth和HealthMana初始化之后
	ApplyGameplayEffectToSelf(DefaultVitalAttributes);
	
}

void AYuraCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority())
	{
		return;
	}
	// 调用ASC上的函数
	UYuraAbilitySystemComponent* YuraASC = CastChecked<UYuraAbilitySystemComponent>(AbilitySystemComponent);
	YuraASC->GrantCharacterAbilities(AbilitiesGrantIngOnStart);
}

void AYuraCharacterBase::ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& GEForAttributes, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GEForAttributes);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GEForAttributes, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}


void AYuraCharacterBase::InitAbilityActorInfo()
{
}

