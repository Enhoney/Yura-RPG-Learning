// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct YURARPG_API FYuraAttributeInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	// 用于在UI中显示的属性名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	// 属性描述
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	// 属性具体的值，这个不会在数据资产中编辑，但是在传递的时候，会从ASC中拿到，再从这里传递过去
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0;


};

/**
 * 
 */
UCLASS()
class YURARPG_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FYuraAttributeInfo FindAttributeInfoByTag(const FGameplayTag& InAttributeTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FYuraAttributeInfo> AttributesInformation;
	
};
