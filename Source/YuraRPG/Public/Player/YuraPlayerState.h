// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
#include "YuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AYuraPlayerState();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** AbilitySystem Interface start*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** AbilitySystem Interface end*/

	// 自己定义的方法，方便获取属性
	UAttributeSet* GetAttributeSet() const;

	FORCEINLINE int32 GetCharacterLevel() const { return Level; };

private:
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	
};
