// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "YuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UYuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static const UYuraAssetManager& Get();

protected:
	// 这个函数很早就会被调用，加载游戏资源的时候就会调用了
	virtual void StartInitialLoading() override;
	
};
