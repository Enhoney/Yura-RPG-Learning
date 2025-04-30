// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "YuraAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
class UAttributeMenuWidgetController;

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 获取OverlayWidgetController
	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* InWorldContextObject);

	// 获取OverlayWidgetController
	UFUNCTION(BlueprintPure, Category = "YuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* InWorldContextObject);

	// 初始化Enemy的初始属性
	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Initialize Character Default Attributes")
	static void InitDefaultAttributes(const UObject* InWorldContextObject, ECharacterClass EnemyClass, float CharacterLevel, UAbilitySystemComponent* TargetASC);

	// 授予敌人通用的能力
	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Grant StartUp Abilities")
	static void GrantStartUpAbilities(const UObject* InWorldContextObject, UAbilitySystemComponent* TargetASC);

	// 从GameMode中拿到数据资产
	UFUNCTION(BlueprintCallable, Category = "YuraAbilitySystemLibrary|Get CharacterClass Info Data")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* InWorldContextObject);

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
};
