// Copyright Muci Yun.


#include "Actor/YuraEffectActor.h"

#include "Components/SphereComponent.h"
#include "YuraAbilitySystemComponent.h"
#include "YuraAttributeSet.h"
#include "AbilitySystemInterface.h"

// Sets default values
AYuraEffectActor::AYuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

void AYuraEffectActor::OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 如果重叠的对象实现了这个接口，就可以拿到上面的ASC
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		// 通过ASC拿到AttributeSet并转换我我们自定义的
		const UYuraAttributeSet* YuraAttributeSet =
			Cast<UYuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UYuraAttributeSet::StaticClass()));

		// 因为它是const的，所以我们是不能直接通过之前的Set直接修改属性的，如果不用GE，就只能使用一些禁忌的方法--去掉const
		// 在实际开发中，不要这样做，切记
		UYuraAttributeSet* MutableYuraAttributeSet = const_cast<UYuraAttributeSet*>(YuraAttributeSet);
		MutableYuraAttributeSet->SetHealth(MutableYuraAttributeSet->GetHealth() + 24.f);

		Destroy();
	}
}

void AYuraEffectActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AYuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AYuraEffectActor::OnOverlapStart);

	Sphere->OnComponentEndOverlap.AddDynamic(this, &AYuraEffectActor::OnOverlapEnd);
	
}

