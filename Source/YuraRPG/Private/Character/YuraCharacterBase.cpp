// Copyright Muci Yun.


#include "YuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

#include "YuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "MotionWarpingComponent.h"

#include "YuraRPG.h"

AYuraCharacterBase::AYuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 忽略相机的碰撞，以免视角忽然拉近
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	// 关闭胶囊体的重叠响应
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	// 启用Mesh的重叠响应
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");

}

UAbilitySystemComponent* AYuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AYuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

FVector AYuraCharacterBase::GetFireSocketLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AYuraCharacterBase::SetWarpTargetFacing(const FVector& TargetLocation)
{
	MotionWarping->AddOrUpdateWarpTargetFromLocation(WarpingTargetName, TargetLocation);
}

void AYuraCharacterBase::Die()
{
	// 这个函数一定是在服务器执行的
	// 武器分离，并保留到世界中
	// 第二个参数--是否在分离相关组件时调用Modify（），按照DeepSeek的说法，就是是否重置移动状态
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	MulticastHandleDeath();
}

void AYuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->SetEnableGravity(true);
	Weapon->SetSimulatePhysics(true);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Disslove();
}

void AYuraCharacterBase::Disslove()
{
	if (IsValid(DissolveEffectMatForMesh))
	{
		// 创建动态材质
		UMaterialInstanceDynamic* MatInsDynamicForMesh = UMaterialInstanceDynamic::Create(DissolveEffectMatForMesh, this);
		// 因为我们这个模型只有一个材质，所以只设置0的即可，如果有多个材质，就得多创建和设置几个了
		GetMesh()->SetMaterial(0, MatInsDynamicForMesh);
		// 开始溶解
		StartMeshDissolveTimeLine(MatInsDynamicForMesh);
	}

	// 武器也得溶解
	if (IsValid(DissolveEffectMatForWeapon))
	{
		// 创建动态材质
		UMaterialInstanceDynamic* MatInsDynamicForWeapon = UMaterialInstanceDynamic::Create(DissolveEffectMatForWeapon, this);
		// 因为我们这个模型只有一个材质，所以只设置0的即可，如果有多个材质，就得多创建和设置几个了
		Weapon->SetMaterial(0, MatInsDynamicForWeapon);
		// 开始溶解
		StartWeaponDissolveTimeLine(MatInsDynamicForWeapon);
	}
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

