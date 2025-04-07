// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "YuraCharacterBase.generated.h"

UCLASS(Abstract)
class YURARPG_API AYuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AYuraCharacterBase();

protected:
	virtual void BeginPlay() override;


protected:

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

};
