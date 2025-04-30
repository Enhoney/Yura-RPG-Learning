// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageFloatingComponent.generated.h"

/**
 * 
 */
UCLASS()
class YURARPG_API UDamageFloatingComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageFloatingText(float Damage, bool bDamageBlock, bool bCriticalHit);
	
};
