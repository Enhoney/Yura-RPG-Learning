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

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool ApplyGameplayEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void StartSinMovement();

	UFUNCTION(BlueprintCallable)
	void StartRotateMovement();

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect Actor")
	bool bDestroyOnEffectApplicated = false;

	// 是否可以施加效果给敌人--在子类中决定
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect Actor")
	bool bApplyToEnemy = false;

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

	// 是否周期性旋转
	UPROPERTY(EditDefaultsOnly, Category = "Loot Effect")
	bool bRotates = false;

	// 旋转速度
	UPROPERTY(EditDefaultsOnly, Category = "Loot Effect")
	float RotateRate = 45.f;

	// 是否在Z轴向正弦运动
	UPROPERTY(EditDefaultsOnly, Category = "Loot Effect")
	bool bSinMovement = false;

	// 振幅
	UPROPERTY(EditDefaultsOnly, Category = "Loot Effect")
	float SinAmplitude = 1.f;

	// 频率
	UPROPERTY(EditDefaultsOnly, Category = "Loot Effect")
	float SinPeriod = 1.f;	// 2PI

	// 运动的时间
	UPROPERTY(EditDefaultsOnly, Category = "Loot Effect")
	float RunningTime = 0.f;

	// 初始位置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Loot Effect", meta = (AllowPrivateAccess = "true"))
	FVector ActorInitialLocation = FVector();


};
