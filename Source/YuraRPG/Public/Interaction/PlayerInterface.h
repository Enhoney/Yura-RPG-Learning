// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class YURARPG_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void AddToExp(int32 ExpToAdd);

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 LevelToAdd);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetCurrentExp() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 FindCurrentLevelByExp(int32 CurExp) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddAttributePoints(int32 AttributePointToAdd);

	UFUNCTION(BlueprintNativeEvent)
	void AddSpellPoints(int32 SpellPointToAdd);
};
