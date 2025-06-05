// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"


class ULoadScreenUserWidget;
class UMVVM_LoadScreen;

/**
 * 
 */
UCLASS()
class YURARPG_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()

public:

protected:

	virtual void BeginPlay() override;

protected:
	// 加载页面的UI类
	UPROPERTY(EditDefaultsOnly, Category = "LoadScreen")
	TSubclassOf<ULoadScreenUserWidget> LoadMenuWidgetClass;

	// UI的实例
	UPROPERTY(BlueprintReadOnly, Category = "LoadScreen")
	TObjectPtr<ULoadScreenUserWidget> LoadMenuWidget;

	// MVVM类
	UPROPERTY(EditDefaultsOnly, Category = "LoadScreen")
	TSubclassOf<UMVVM_LoadScreen> LoadScreenModelClass;

	// MVVM实例
	UPROPERTY(BlueprintReadOnly, Category = "LoadScreen")
	TObjectPtr<UMVVM_LoadScreen> LoadScreenModel;
	
};
