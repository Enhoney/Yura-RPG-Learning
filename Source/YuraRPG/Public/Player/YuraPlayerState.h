// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "YuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

class ULevelUpInfo;

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

	/** Level*/
	FORCEINLINE int32 GetCharacterLevel() const { return Level; };
	void SetCharacterLevel(int32 NewLevel);
	void AddToCharacterLevel(int32 LevelToAdd);

	/** EXP*/
	FORCEINLINE int32 GetPlayerExp() const { return Exp; };
	void SetExp(int32 NewExp);
	void AddToExp(int32 ExpToAdd);

	/** Attribute Point*/
	FORCEINLINE int32 GetAttributePoint() const { return AttributePoint; };
	void SetAttributePoint(int32 NewAttributePoint);
	void AddToAttributePoint(int32 AttributePointToAdd);
	void ConsumeAttributePoint(int32 AttributePointToUse);

	/** Spell Point*/
	FORCEINLINE int32 GetSpellPoint() const { return SpellPoint; };
	void SetSpellPoint(int32 NewSpellPoint);
	void AddToSpellPoint(int32 SpellPointToAdd);
	void ConsumeSpellPoint(int32 SpellPointToUse);

private:
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_Exp(int32 OldExp);

	UFUNCTION()
	void OnRep_AttributePoint(int32 OldExp);

	UFUNCTION()
	void OnRep_SpellPoint(int32 OldExp);

public:

	FPlayerStateChangedSingnature OnPlayerLevelChangedDelegate;

	FPlayerStateChangedSingnature OnPlayerExpChangedDelegate;

	FPlayerStateChangedSingnature OnAttributePointChangedDelegate;

	FPlayerStateChangedSingnature OnSpellPointChangedDelegate;

	UPROPERTY(EditDefaultsOnly, Category = "Leveling Up")
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

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

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoint)
	int32 AttributePoint = 5;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoint)
	int32 SpellPoint = 3;
	
};
