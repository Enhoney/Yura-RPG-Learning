// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "PassiveNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:

	UPassiveNiagaraComponent() { bAutoActivate = false; }

	// 因为不确定这个BeginPlay是否会在ASC Init之前还是之后执行，所以，我们最好提供一个手动绑代理的方法，在我们Init之后手动调用
	void InitPassiveNiagaraComponent(const FGameplayTag& InPassivaAbilityTag);

protected:
	UPROPERTY(VisibleAnywhere)
	FGameplayTag PassiveAbilityTag = FGameplayTag();

	void HandlePassiveNiagara(const FGameplayTag& InPassiveAbilityTag, bool bInIsEquipped);

	UFUNCTION()
	void OnOwnerDeath(AActor* OwnerActor);
};
