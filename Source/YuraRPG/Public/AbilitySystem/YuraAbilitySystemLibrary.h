// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "YuraAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
class AYuraHUD;

struct FWidgetControllerParam;

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 构建WidgetControllerParam
	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|WidgetController")
	static bool MakeWidgetControllerParam(const UObject* InWorldContextObject, FWidgetControllerParam& OutParams, AYuraHUD*& OutHUD);

	// 获取OverlayWidgetController
	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "InWorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* InWorldContextObject);

	// 获取OverlayWidgetController
	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "InWorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* InWorldContextObject);

	// 获取SpellMenuWidgetControlelr
	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "InWorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* InWorldContextObject);

	// 初始化Enemy的初始属性
	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Initialize Character Default Attributes")
	static void InitDefaultAttributes(const UObject* InWorldContextObject, ECharacterClass EnemyClass, float CharacterLevel, UAbilitySystemComponent* TargetASC);

	// 授予敌人通用的能力，以及独有的能力
	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Grant StartUp Abilities")
	static void GrantStartUpAbilities(const UObject* InWorldContextObject, UAbilitySystemComponent* TargetASC, ECharacterClass EnemyClass, int32 EnemyLevel);

	// 查找对应的经验点
	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Get Exp Reward for Class and Level")
	static int32 FindEnemyExpReward(const UObject* InWorldContextObject, ECharacterClass InCharacterClass, int32 InCharacterLevel);

	// 从GameMode中拿到数据资产
	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Get CharacterClass Info Data")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* InWorldContextObject);

	// 获取GameMode上面的AbilityInfo
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "YuraAbilitySystemLibrary|AbilityInfo")
	static UAbilityInfo* GetAbilityInfoOnGameMode(const UObject* InWorldContextObject);

	// 获取能力的TypeTag--主动还是被动，还是固有技能--用于读档和存档
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "YuraAbilitySystemLibrary|AbilityInfo")
	static FGameplayTag GetAbilityTypeTagFromSpec(const UObject* InWorldContextObject, const FGameplayAbilitySpec& InAbilitySpec);

	/** 
	 * 用于自定义的GameplayEffectContext
	 */

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static bool IsDamageBlock(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static float GetDebuffBaseDamage(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static FGameplayTag GetDamageTypeTag(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static FVector GetKnockbackVector(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static float GetRadialInnerRadius(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static float GetRadialOuterRadius(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static FVector GetRadialCenterLocation(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetDamageBlock(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool bInDamageBlock);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool bInCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool bInIsSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetDebuffBaseDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, float InDebuffBaseDamage);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, float InDebuffDuration);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, float InDebuffFrequency);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetDamageTypeTag(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, const FGameplayTag& InDamageTypeTag);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, const FVector& InDeathImpulse);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetKnockbackVector(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, const FVector& InKnockbackVector);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool bInIsRadialDamage);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetRadialInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, float InRadialInnerRadius);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetRadialOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, float InRadialOuterRadius);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetRadialCenterLocation(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, const FVector& InRadialCenterLocation);

	// 获取范围内的指定目标
	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Gameplay Mechinces")
	static void GetLivePlayersWithinRadius(const UObject* InWorldContextObject, TArray<AActor*>& OutOverlapActors , 
		const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& InSphereCenterLocation);

	// 获取最近的几个Actor
	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Gameplay Mechinces")
	static void GetClosetActors(int32 InNumToGet, const FVector& InOriginLocation, 
		const TArray<AActor*>& InOriginActors, TArray<AActor*>& OutActors);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "YuraAbilitySystemLibrary|Combat")
	static bool IsNotFriend(const AActor* SourceActor, const AActor* TargetActor);

	// 使用DamageEffectParams施加伤害
	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|ApplyDamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffectByParams(const FDamageEffectParams& Params);

	// 获取均匀分布的角度
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "YuraAbilitySystemLibrary|Algorithm")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumToDivide);
	// 获取均匀分布的向量
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "YuraAbilitySystemLibrary|Algorithm")
	static TArray<FVector> EvenlySpacedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumToDivide);
};
