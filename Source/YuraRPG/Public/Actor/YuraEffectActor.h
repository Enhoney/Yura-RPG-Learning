// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YuraEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class YURARPG_API AYuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AYuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool ApplyGameplayEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect Actor")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect Actor")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

};
