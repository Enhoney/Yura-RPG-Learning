// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "YuraEffectActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnBeginOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovePolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class YURARPG_API AYuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AYuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool ApplyGameplayEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect Actor")
	bool bDresroyOnEffectRemoval = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect Actor")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect Actor")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect Actor")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect Actor")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect Actor")
	EEffectApplicationPolicy  DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect Actor")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect Actor")
	EEffectRemovePolicy InfiniteEffectRemovePolicy = EEffectRemovePolicy::DoNotRemove;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effect Actor")
	float ActorLevel = 1.0f;

private:

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
};
