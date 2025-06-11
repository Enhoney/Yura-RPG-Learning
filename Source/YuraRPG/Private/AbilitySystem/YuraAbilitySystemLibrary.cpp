// Copyright Muci Yun.


#include "AbilitySystem/YuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/YuraHUD.h"
#include "UI/WidgetController/YuraWidgetController.h"
#include "YuraPlayerState.h"
#include "Game/YuraGameModeBase.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "YuraGameplayTags.h"
#include "YuraAbilityTypes.h"
#include "AbilitySystem/YuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Engine/OverlapResult.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"


bool UYuraAbilitySystemLibrary::MakeWidgetControllerParam(const UObject* InWorldContextObject, FWidgetControllerParam& OutParams, AYuraHUD*& OutHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(InWorldContextObject, 0))
	{
		if (AYuraHUD* YuraHUD = Cast<AYuraHUD>(PC->GetHUD()))
		{
			AYuraPlayerState* PS = PC->GetPlayerState<AYuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutParams.PlayerController = PC;
			OutParams.PlayerState = PS;
			OutParams.AbilitySystemComponent = ASC;
			OutParams.AttributeSet = AS;

			OutHUD = YuraHUD;
			return true;
		}
	}

	return false;
}

UOverlayWidgetController* UYuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* InWorldContextObject)
{
	FWidgetControllerParam WidgetControllerParams;
	AYuraHUD* YuraHUD = nullptr;
	if (MakeWidgetControllerParam(InWorldContextObject, WidgetControllerParams, YuraHUD))
	{
		return YuraHUD->GetOverlayWidgetController(WidgetControllerParams);

	}

	return nullptr;
}

UAttributeMenuWidgetController* UYuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* InWorldContextObject)
{
	FWidgetControllerParam WidgetControllerParams;
	AYuraHUD* YuraHUD = nullptr;
	if (MakeWidgetControllerParam(InWorldContextObject, WidgetControllerParams, YuraHUD))
	{
		return YuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		
	}

	return nullptr;

}

USpellMenuWidgetController* UYuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* InWorldContextObject)
{
	FWidgetControllerParam WidgetControllerParams;
	AYuraHUD* YuraHUD = nullptr;
	if (MakeWidgetControllerParam(InWorldContextObject, WidgetControllerParams, YuraHUD))
	{
		return YuraHUD->GetSpellMenuWidgetController(WidgetControllerParams);

	}

	return nullptr;
}

void UYuraAbilitySystemLibrary::InitDefaultAttributes(const UObject* InWorldContextObject, ECharacterClass EnemyClass, float CharacterLevel, UAbilitySystemComponent* TargetASC)
{
	AYuraGameModeBase* YuraGameMode = Cast<AYuraGameModeBase>(UGameplayStatics::GetGameMode(InWorldContextObject));

	// 因为在客户端是拿不到GameMode的
	if (YuraGameMode == nullptr)
	{
		return;
	}

	// 用于设置SourceObject
	const AActor* AvatorActor = TargetASC->GetAvatarActor();

	// 获取CharacterInfo
	UCharacterClassInfo* CharacterInfo = YuraGameMode->DefaultEnemyInfo;

	const FCharacterClassDefaultInfo DefaultCharacterInfo = CharacterInfo->GetDerfaultCharacterInfo(EnemyClass);
	// 初始化Primary Attribute
	FGameplayEffectContextHandle PrimaryAttributeContextHandle = TargetASC->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(AvatorActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = TargetASC->MakeOutgoingSpec(DefaultCharacterInfo.DefaultPrimaryAttribute, CharacterLevel, PrimaryAttributeContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());
	// 初始化次要属性
	FGameplayEffectContextHandle SecondaryAttributeContextHandle = TargetASC->MakeEffectContext();
	SecondaryAttributeContextHandle.AddSourceObject(AvatorActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = TargetASC->MakeOutgoingSpec(CharacterInfo->DefaultSecondaryAttribute, CharacterLevel, SecondaryAttributeContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());
	// 初始化重要属性
	FGameplayEffectContextHandle VitalAttributeContextHandle = TargetASC->MakeEffectContext();
	VitalAttributeContextHandle.AddSourceObject(AvatorActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = TargetASC->MakeOutgoingSpec(CharacterInfo->DefaultVitalAttribute, CharacterLevel, VitalAttributeContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}

void UYuraAbilitySystemLibrary::GrantStartUpAbilities(const UObject* InWorldContextObject, UAbilitySystemComponent* TargetASC, ECharacterClass EnemyClass, int32 EnemyLevel)
{
	AYuraGameModeBase* YuraGameMode = Cast<AYuraGameModeBase>(UGameplayStatics::GetGameMode(InWorldContextObject));

	// 因为在客户端是拿不到GameMode的
	if (YuraGameMode == nullptr)
	{
		return;
	}

	// 获取CharacterInfo
	UCharacterClassInfo* CharacterInfo = YuraGameMode->DefaultEnemyInfo;

	for (TSubclassOf<UGameplayAbility> CommonAbilityClass : CharacterInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CommonAbilityClass, 1);
		TargetASC->GiveAbility(AbilitySpec);
	}

	// 授予独有能力
	const FCharacterClassDefaultInfo DefaultCharacterInfo = CharacterInfo->GetDerfaultCharacterInfo(EnemyClass);
	for (TSubclassOf<UGameplayAbility> UniqueAbilityClass : DefaultCharacterInfo.UniqueAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(UniqueAbilityClass, EnemyLevel);
		TargetASC->GiveAbility(AbilitySpec);
	}
}

int32 UYuraAbilitySystemLibrary::FindEnemyExpReward(const UObject* InWorldContextObject, ECharacterClass InCharacterClass, int32 InCharacterLevel)
{
	AYuraGameModeBase* YuraGameMode = Cast<AYuraGameModeBase>(UGameplayStatics::GetGameMode(InWorldContextObject));

	// 因为在客户端是拿不到GameMode的
	if (YuraGameMode == nullptr)
	{
		return 0;
	}

	// 获取CharacterInfo
	UCharacterClassInfo* CharacterInfo = YuraGameMode->DefaultEnemyInfo;

	const FCharacterClassDefaultInfo DefaultCharacterInfo = CharacterInfo->GetDerfaultCharacterInfo(InCharacterClass);
	const float ExpReward =  DefaultCharacterInfo.ExpReward.GetValueAtLevel(InCharacterLevel);

	return static_cast<int32>(ExpReward);
}

UCharacterClassInfo* UYuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* InWorldContextObject)
{
	AYuraGameModeBase* YuraGameMode = Cast<AYuraGameModeBase>(UGameplayStatics::GetGameMode(InWorldContextObject));
	if (YuraGameMode == nullptr)
	{
		return nullptr;
	}

	// 获取CharacterInfo
	UCharacterClassInfo* CharacterInfo = YuraGameMode->DefaultEnemyInfo;
	return CharacterInfo;
}

UAbilityInfo* UYuraAbilitySystemLibrary::GetAbilityInfoOnGameMode(const UObject* InWorldContextObject)
{
	AYuraGameModeBase* YuraGameMode = Cast<AYuraGameModeBase>(UGameplayStatics::GetGameMode(InWorldContextObject));
	if (YuraGameMode == nullptr)
	{
		return nullptr;
	}

	// 获取CharacterInfo
	UAbilityInfo* AbilityInfo = YuraGameMode->DefaultAbilityInfo;
	return AbilityInfo;
}

ULootTiers* UYuraAbilitySystemLibrary::GetLootTiersOnGameMode(const UObject* InWorldContextObject)
{
	AYuraGameModeBase* YuraGameMode = Cast<AYuraGameModeBase>(UGameplayStatics::GetGameMode(InWorldContextObject));
	if (YuraGameMode == nullptr)
	{
		return nullptr;
	}

	// 获取LootTiers
	ULootTiers* LootTiers = YuraGameMode->LootTiers;
	return LootTiers;
}

FGameplayTag UYuraAbilitySystemLibrary::GetAbilityTypeTagFromSpec(const UObject* InWorldContextObject, const FGameplayAbilitySpec& InAbilitySpec)
{
	FGameplayTag OutAbilityTypeTag = FYuraGameplayTags::Get().Ability_Type_None;

	FGameplayTag AbilityTag = UYuraAbilitySystemComponent::GetAbilityTagFromSpec(InAbilitySpec);
	UAbilityInfo* AbilityInfo = GetAbilityInfoOnGameMode(InWorldContextObject);
	FGameplayTag AbilityTypeTag = AbilityInfo->FindAbilityInfoByTag(AbilityTag).AbilityTypeTag;

	if (AbilityTypeTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability.Type"))))
	{
		OutAbilityTypeTag = AbilityTypeTag;
	}
	
	return OutAbilityTypeTag;
}

bool UYuraAbilitySystemLibrary::IsDamageBlock(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->IsDamageBlock();
	}

	return false;
}

bool UYuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->IsCriticalHit();
	}

	return false;
}

bool UYuraAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->IsSuccessfulDebuff();
	}

	return false;
}


float UYuraAbilitySystemLibrary::GetDebuffBaseDamage(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->GetDebuffBaseDamage();
	}

	return 0.f;
}

float UYuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->GetDebuffDuration();
	}

	return 0.f;
}

float UYuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->GetDebuffFrequency();
	}

	return 0.0f;
}

FGameplayTag UYuraAbilitySystemLibrary::GetDamageTypeTag(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->GetDamageTypeTag();
	}

	return FGameplayTag();
}

FVector UYuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->GetDeathImpulse();
	}

	return FVector::ZeroVector;
}

FVector UYuraAbilitySystemLibrary::GetKnockbackVector(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->GetKnockbackVector();
	}

	return FVector::ZeroVector;
}

bool UYuraAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->IsRadialDamage();
	}

	return false;
}

float UYuraAbilitySystemLibrary::GetRadialInnerRadius(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->GetRadialInnerRadius();
	}

	return 0.f;
}

float UYuraAbilitySystemLibrary::GetRadialOuterRadius(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->GetRadialOuterRadius();
	}

	return 0.f;
}

FVector UYuraAbilitySystemLibrary::GetRadialCenterLocation(const FGameplayEffectContextHandle& EffectContext)
{
	const FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	const FYuraGameplayEffectContext* YuraContext = static_cast<const FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		return YuraContext->GetRadialCenterLocation();
	}

	return FVector::ZeroVector;
}

void UYuraAbilitySystemLibrary::SetDamageBlock(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool bInDamageBlock)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetIsDamageBlock(bInDamageBlock);
	}

}

void UYuraAbilitySystemLibrary::SetCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool bInCriticalHit)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetIsCriticalHit(bInCriticalHit);
	}
}

void UYuraAbilitySystemLibrary::SetIsSuccessfulDebuff(UPARAM(ref)FGameplayEffectContextHandle& EffectContext, bool bInIsSuccessfulDebuff)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

void UYuraAbilitySystemLibrary::SetDebuffBaseDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContext, float InDebuffBaseDamage)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetDebuffBaseDamage(InDebuffBaseDamage);
	}
}

void UYuraAbilitySystemLibrary::SetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& EffectContext, float InDebuffDuration)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UYuraAbilitySystemLibrary::SetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& EffectContext, float InDebuffFrequency)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

void UYuraAbilitySystemLibrary::SetDamageTypeTag(UPARAM(ref)FGameplayEffectContextHandle& EffectContext, const FGameplayTag& InDamageTypeTag)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetDamageTypeTag(InDamageTypeTag);
	}
}

void UYuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContext, const FVector& InDeathImpulse)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UYuraAbilitySystemLibrary::SetKnockbackVector(FGameplayEffectContextHandle& EffectContext, const FVector& InKnockbackVector)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetKnockbackVector(InKnockbackVector);
	}
}

void UYuraAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContext, bool bInIsRadialDamage)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UYuraAbilitySystemLibrary::SetRadialInnerRadius(FGameplayEffectContextHandle& EffectContext, float InRadialInnerRadius)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetRadialInnerRadius(InRadialInnerRadius);
	}
}

void UYuraAbilitySystemLibrary::SetRadialOuterRadius(FGameplayEffectContextHandle& EffectContext, float InRadialOuterRadius)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetRadialOuterRadius(InRadialOuterRadius);
	}
}

void UYuraAbilitySystemLibrary::SetRadialCenterLocation(FGameplayEffectContextHandle& EffectContext, const FVector& InRadialCenterLocation)
{
	FGameplayEffectContext* Context = EffectContext.Get();
	// 转化，因为Cast<>只能用于UObject及其子类，所以这里只能用C++原生的static_cast
	FYuraGameplayEffectContext* YuraContext = static_cast<FYuraGameplayEffectContext*>(Context);

	if (YuraContext)
	{
		YuraContext->SetRadialCenterLocation(InRadialCenterLocation);
	}
}

void UYuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* InWorldContextObject, TArray<AActor*>& OutOverlapActors,
	const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& InSphereCenterLocation)
{
	// 用于碰撞查询的参数
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);

	// 记录所有重叠的结果
	TArray<FOverlapResult> OverlapResults;
	if (UWorld* World =  GEngine->GetWorldFromContextObject(InWorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(OverlapResults, InSphereCenterLocation, FQuat::Identity, 
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), 
			FCollisionShape::MakeSphere(Radius), CollisionQueryParams);

		for (FOverlapResult& OverResult : OverlapResults)
		{
			// 是否实现了接口的第二种判断方法，一共有三种方法
			if (OverResult.GetActor()->Implements<UCombatInterface>())
			{
				// 调用接口函数
				const bool bIsLive = !ICombatInterface::Execute_IsDead(OverResult.GetActor());

				if (bIsLive)
				{
					// 直接传递OverResult.GetActor()也是一样的
					OutOverlapActors.AddUnique(ICombatInterface::Execute_GetAvatar(OverResult.GetActor()));
				}
			}
		}
		
	}
}

void UYuraAbilitySystemLibrary::GetClosetActors(int32 InNumToGet, const FVector& InOriginLocation, 
	const TArray<AActor*>& InOriginActors, TArray<AActor*>& OutActors)
{
	// 如果数量不足
	if (InNumToGet >= InOriginActors.Num())
	{
		OutActors = InOriginActors;
		return;
	}

	// 方便删除用的
	TArray<AActor*> TempOrignActors = InOriginActors;
	int32 NumFound = 0;
	while (NumFound < InNumToGet)
	{
		// 一般来说不会这样，为了保险还是加上
		if (TempOrignActors.Num() == 0) break;

		// 这是获取各个类型最大极限值的通用方法
		double ClosetDistance = TNumericLimits<double>::Max();
		int32 ClosedActorIndex = 0;
		// 每次遍历完，都能找到一个最近的
		for (int32 Index = 0; Index < TempOrignActors.Num(); ++Index)
		{
			// 计算到目标点的距离
			double DistanceToOriginActor = FVector::Distance(TempOrignActors[Index]->GetActorLocation(), InOriginLocation);
			if (DistanceToOriginActor < ClosetDistance)
			{
				ClosetDistance = DistanceToOriginActor;
				ClosedActorIndex = Index;
			}
		}
		// 然后，把它添加到OutActors中，并从Temp中移除，NumFound加一
		OutActors.AddUnique(TempOrignActors[ClosedActorIndex]);
		TempOrignActors.RemoveAt(ClosedActorIndex);
		++NumFound;

	}
}

bool UYuraAbilitySystemLibrary::IsNotFriend(const AActor* SourceActor, const AActor* TargetActor)
{
	const FName PlayerTag = FName("YuraCharacter.Player");
	const FName EnemyTag = FName("YuraCharacter.Enemy");

	if (SourceActor->ActorHasTag(PlayerTag) && TargetActor->ActorHasTag(PlayerTag) ||
		SourceActor->ActorHasTag(EnemyTag) && TargetActor->ActorHasTag(EnemyTag))
	{
		return false;
	}
	
	return true;
}

FGameplayEffectContextHandle UYuraAbilitySystemLibrary::ApplyDamageEffectByParams(const FDamageEffectParams& Params)
{
	const FYuraGameplayTags YuraTags = FYuraGameplayTags::Get();
	const AActor* AvatarActor = Params.SourceASC->GetAvatarActor();

	FGameplayEffectContextHandle DamageEffectContext = Params.SourceASC->MakeEffectContext();
	DamageEffectContext.AddSourceObject(AvatarActor);
	// 设置死亡冲量
	SetDeathImpulse(DamageEffectContext, Params.DeathImpulse);
	// 设置击退冲量
	SetKnockbackVector(DamageEffectContext, Params.KnockbackVector);

	// 设置范围伤害
	SetIsRadialDamage(DamageEffectContext, Params.bIsRadialDamge);
	SetRadialInnerRadius(DamageEffectContext, Params.RadialInnerRadius);
	SetRadialOuterRadius(DamageEffectContext, Params.RadialOuterRadius);
	SetRadialCenterLocation(DamageEffectContext, Params.RadialCenterLocation);

	const FGameplayEffectSpecHandle EffectSpec = Params.SourceASC->MakeOutgoingSpec(Params.DamageGameplayEffectClass, Params.AbilityLevel, DamageEffectContext);

	// 基础伤害
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpec, Params.DamageType, Params.AbilityBaseDamage);
	// Debuff
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpec, YuraTags.DebuffParam_Chance, Params.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpec, YuraTags.DebuffParam_Duration, Params.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpec, YuraTags.DebuffParam_Frequency, Params.DebuffFrequency);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpec, YuraTags.DebuffParam_BaseDamage, Params.DebuffBaseDamage);

	Params.TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data);

	return DamageEffectContext;
}

TArray<FRotator> UYuraAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumToDivide)
{
	TArray<FRotator> OutRotators;

	// 只有一个就直接从中间生成
	if (NumToDivide == 1)
	{
		OutRotators.Add(Forward.Rotation());
	}
	else if (NumToDivide > 1)
	{
		// 第0个Vector
		FVector LeftVector = Forward.RotateAngleAxis(-Spread / 2, Axis);
		// 最后一个Vector
		FVector RightVector = Forward.RotateAngleAxis(Spread / 2, Axis);
		// DeltaSpread
		const float DeltaSpread = Spread / (NumToDivide - 1);
		for (int32 i = 0; i < NumToDivide; i++)
		{
			const FVector Direction = LeftVector.RotateAngleAxis(DeltaSpread * i, Axis);
			OutRotators.Add(Direction.Rotation());

		}
	}

	return OutRotators;
}

TArray<FVector> UYuraAbilitySystemLibrary::EvenlySpacedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumToDivide)
{
	TArray<FVector> OutVectors;

	// 只有一个就直接从中间生成
	if (NumToDivide == 1)
	{
		OutVectors.Add(Forward);
	}
	else if (NumToDivide > 1)
	{
		// 第0个Vector
		FVector LeftVector = Forward.RotateAngleAxis(-Spread / 2, Axis);
		// 最后一个Vector
		FVector RightVector = Forward.RotateAngleAxis(Spread / 2, Axis);
		// DeltaSpread
		const float DeltaSpread = Spread / (NumToDivide - 1);
		for (int32 i = 0; i < NumToDivide; i++)
		{
			const FVector Direction = LeftVector.RotateAngleAxis(DeltaSpread * i, Axis);
			OutVectors.Add(Direction);

		}
	}

	return OutVectors;
}

