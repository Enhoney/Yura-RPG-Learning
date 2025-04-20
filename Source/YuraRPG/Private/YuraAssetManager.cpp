// Copyright Muci Yun.


#include "YuraAssetManager.h"

#include "YuraGameplayTags.h"

const UYuraAssetManager& UYuraAssetManager::Get()
{
	check(GEngine);
	// 需要在虚幻编辑的项目设置中进行配置
	UYuraAssetManager* YuraAssetManager = Cast<UYuraAssetManager>(GEngine->AssetManager);

	return *YuraAssetManager;
}

void UYuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// 这就是一个绝佳的注册GameplayTag的地方
	FYuraGameplayTags::InitializeGameplayTags();

}
