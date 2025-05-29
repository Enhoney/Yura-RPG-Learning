// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/YuraDamageGameplayAbility.h"
#include "YuraFireBlast.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraFireBlast : public UYuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	// 获取当前描述
	virtual FString GetCurrentLevelDescription(int Level) override;
	// 获取下一级的描述
	virtual FString GetNextLevelDescription(int Level) override;

protected:

	UPROPERTY(EditDefaultsOnly)
	int32 NumToSpawn = 12;
	
};
