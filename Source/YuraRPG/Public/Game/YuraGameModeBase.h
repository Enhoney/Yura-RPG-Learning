// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "YuraGameModeBase.generated.h"

class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Enemy 初始化属性的资产
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Character Attribute Default")
	TObjectPtr<UCharacterClassInfo> DefaultEnemyInfo;
	
};
