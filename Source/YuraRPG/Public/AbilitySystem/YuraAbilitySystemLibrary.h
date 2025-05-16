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

	/** 
	 * 用于自定义的GameplayEffectContext
	 */

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static bool IsDamageBlock(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetDamageBlock(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool bInDamageBlock);

	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Yura GameplayEffect Context")
	static void SetCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool bInCriticalHit);


	// 获取范围内的指定目标
	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Gameplay Mechinces")
	static void GetLivePlayersWithinRadius(const UObject* InWorldContextObject, TArray<AActor*>& OutOverlapActors , 
		const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& InSphereCenterLocation);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "YuraAbilitySystemLibrary|Combat")
	static bool IsNotFriend(const AActor* SourceActor, const AActor* TargetActor);


};
