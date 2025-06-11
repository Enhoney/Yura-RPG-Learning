// Copyright Muci Yun.


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItensToSpawn() const
{
	TArray<FLootItem> OutLootItems;

	for (const FLootItem& LootItem : LootTiers)
	{
		for (int32 i = 0; i < LootItem.MaxNumToSpawn; ++i)
		{
			if (FMath::RandRange(1, 100) <= LootItem.ChanceToSpawn * 100)
			{
				FLootItem NewLootItem;
				NewLootItem.LootClass = LootItem.LootClass;
				NewLootItem.bLootLevelOverride = true;
				OutLootItems.Add(NewLootItem);
			}
		}
	}

	return OutLootItems;
}
