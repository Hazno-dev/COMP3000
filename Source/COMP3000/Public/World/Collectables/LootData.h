// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "DungeonGeneration/TileBase.h"
#include "UObject/NoExportTypes.h"
#include "LootData.generated.h"

USTRUCT(BlueprintType)
struct FLootTable : public FTableRowBase
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Tier;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MinXP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxXP;

	FLootTable()
	{
		Tier = 0;
		MinXP = 0;
		MaxXP = 10;
	}
	
};