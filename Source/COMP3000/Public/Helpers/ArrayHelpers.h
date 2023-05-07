// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DungeonGeneration/Prefabinator.h"
#include "AI/EnemyData.h"
#include "World/Collectables/LootCrate.h"
#include "ArrayHelpers.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UArrayHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template<typename T>
	static void ShuffleArray(TArray<T>& Array, FRandomStream& SeedStream);

	UFUNCTION(BlueprintCallable, Category = "Array Helpers")
	static void ShuffleArrayOfLootCrates(TArray<ALootCrate*>& Array, FRandomStream& SeedStream);

	UFUNCTION(BlueprintCallable, Category = "Array Helpers")
	static void ShuffleArrayOfPrefabinators(TArray<APrefabinator*>& Array, FRandomStream& SeedStream);

	UFUNCTION(BlueprintCallable, Category = "Array Helpers")
	static void ShuffleArrayOfEnemyData(TArray<FEnemyData>& Array, FRandomStream& SeedStream);

	
};
