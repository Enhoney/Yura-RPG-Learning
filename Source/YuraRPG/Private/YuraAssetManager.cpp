// Copyright Muci Yun.


#include "YuraAssetManager.h"

#include "YuraGameplayTags.h"

#include "AbilitySystemGlobals.h"

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

	// 不加上这个可能在使用GAS多人游戏的时候，遇到一些难以排查的BUG
	UAbilitySystemGlobals::Get().InitGlobalData();

}
