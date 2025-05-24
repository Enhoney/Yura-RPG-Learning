// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/YuraDamageGameplayAbility.h"
#include "YuraBeamSpell.generated.h"

class ACharacter;

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraBeamSpell : public UYuraDamageGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Beam")
	void StoreCursorHitInfo(const FHitResult& CursorHitResult);

	// Owner的信息
	UFUNCTION(BlueprintCallable, Category = "Beam")
	void StoreOwnerOwnerVariables();

	// 修正目标位置
	UFUNCTION(BlueprintCallable, Category = "Beam")
	void FirstTraceTarget(const FVector& BeamTargetLocation);

	// 获取额外的攻击目标
	UFUNCTION(BlueprintCallable, Category = "Beam")
	void StoreAdditionalTargets(TArray<AActor*>& OutTargets);

protected:
	// 鼠标下方位置--从PlayerController中获取，用于光束的结束位置
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector CursorHitLocation;


	// 鼠标点击的目标--从PlayerController中获取，用于向目标施加伤害
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> CursorHitActor;

	// 玩家控制器
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	// 玩家的Character
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<ACharacter> OwnerPlayerCharacter;

	// 单次释放技能最多电击的目标数量
	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	int32 MaxNumShocked = 3;
};
