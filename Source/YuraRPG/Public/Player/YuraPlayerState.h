// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
#include "YuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerStateChangedSingnature, int32);

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
	void SetCharacterLevel(int32 NewLevel);
	void AddToCharacterLevel(int32 LevelToAdd);

	/** EXP*/
	FORCEINLINE int32 GetPlayerExp() const { return Exp; };
	void SetExp(int32 NewExp);
	void AddToExp(int32 ExpToAdd);

private:
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_Exp(int32 OldExp);

public:

	FPlayerStateChangedSingnature OnPlayerLevelChangedDelegate;

	FPlayerStateChangedSingnature OnPlayerExpChangedDelegate;

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Exp)
	int32 Exp = 0;
	
};
