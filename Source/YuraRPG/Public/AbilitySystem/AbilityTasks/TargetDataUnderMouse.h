// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataUnderMouseSinature, const FGameplayAbilityTargetDataHandle &, DataHandle);

/**
 * 
 */
UCLASS()
class YURARPG_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	// 仅蓝图内部使用，这是一个工厂方法
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "GetTargetDataUnderMouse",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

private:
	// 自动调用的
	virtual void Activate() override;

	/** 客户端发送数据*/
	void SendMouseCursorData();

	/** 服务器接收数据回调*/
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);

public:
	// 输出引脚
	UPROPERTY(BlueprintAssignable)
	FTargetDataUnderMouseSinature ValidData;
	
};
