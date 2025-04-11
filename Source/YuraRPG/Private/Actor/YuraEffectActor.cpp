// Copyright Muci Yun.


#include "Actor/YuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

// Sets default values
AYuraEffectActor::AYuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
}


void AYuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AYuraEffectActor::ApplyGmaeplayEffectToActor(AActor* TargetActor)
{
	// 查找ASC，我们有两种方法，一种是直接调用接口的GetAbilitySystemComponent函数
	// 还有一种更通用的，由蓝图静态函数库提供的方法，这种更好更通用，如果我们没有实现这个接口，但是也为它添加了ASC，
	// 在这种情况下，依然可以查找到（通过FindComponentByClass）
	// 当然，如果你习惯于为每一个添加ASC的类都实现这个接口，调用接口的方法也是没问题的
	UAbilitySystemComponent* TargrtASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargrtASC == nullptr)
	{
		return;
	}

	check(InstantGameplayEffectClass);
	FGameplayEffectContextHandle GameplayEffectContextHandle = TargrtASC->MakeEffectContext();
	// 效果的直接来源
	GameplayEffectContextHandle.AddSourceObject(this);
	// 生成Spec
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = TargrtASC->MakeOutgoingSpec(InstantGameplayEffectClass, 1.0f, GameplayEffectContextHandle);
	
	// 施加效果
	TargrtASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());

}

