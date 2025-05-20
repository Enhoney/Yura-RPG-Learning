// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "DebuffNiagaraComponent.generated.h"


/**
 * 
 */
UCLASS()
class YURARPG_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
	

public:
	UDebuffNiagaraComponent() { bAutoActivate = false; }


protected:
	// 这个是绑定代理的地方
	void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag = FGameplayTag();

	void HandleDebuffNiagara(const FGameplayTag InDebuffTag, int32 NewCount);

	UFUNCTION()
	void OnOwnerDeath(AActor* OwnerActor);
};
