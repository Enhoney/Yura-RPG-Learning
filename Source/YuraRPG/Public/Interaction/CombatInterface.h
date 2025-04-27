// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class YURARPG_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 不是必须重写的
	// 获取角色等级
	virtual int32 GetCharacterLevel();

	// 获取SocketLocation
	virtual FVector GetFireSocketLocation();

	// 设置Wrap位置朝向
	virtual void SetWarpTargetFacing(const FVector& TargetLocation);

	// 获取受击动画
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UAnimMontage* GetHitReactAnimMontage();

	virtual void Die() = 0;
};
