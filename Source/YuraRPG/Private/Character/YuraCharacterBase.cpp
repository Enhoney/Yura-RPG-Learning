// Copyright Muci Yun.


#include "YuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

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
	
}

void AYuraCharacterBase::ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& GEForAttributes, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GEForAttributes);

	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();

	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GEForAttributes, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}


void AYuraCharacterBase::InitAbilityActorInfo()
{
}

