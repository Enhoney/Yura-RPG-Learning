// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,	/** 法师*/
	Warrior,		/** 战士*/
	Ranger			/** 游侠*/
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Default CharacterClassInfo")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttribute;
};

/**
 * 
 */
UCLASS()
class YURARPG_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	FCharacterClassDefaultInfo GetDerfaultCharacterInfo(ECharacterClass CharacterClass) { return CharacterClassInformation.FindChecked(CharacterClass); }

	UPROPERTY(EditDefaultsOnly, Category = "Class Character Info")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Common Ability")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Common Character Info")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Common Character Info")
	TSubclassOf<UGameplayEffect> DefaultVitalAttribute;
	
};
