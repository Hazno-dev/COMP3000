// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "LevelGeneratorV2.h"
#include "DungeonGeneration/TileBase.h"
#include "GameFramework/Actor.h"
#include "DungeonEnemyManager.generated.h"

UCLASS()
class COMP3000_API ADungeonEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonEnemyManager();
	
	/** Amount of enemies to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 MaxEnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Tier0LootCrateCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Tier1LootCrateCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Tier2LootCrateCount;

	UFUNCTION()
	void SubAllTilesGenerated(ALevelGeneratorV2* InGenerator, TArray<ATileBase*> InTileBases);
	
	UFUNCTION()
	void SubAllLevelsGenerated();

	// This function initializes the spawning of enemies and prefabs in the generated dungeon.
	// It collects all required actors, spawns prefabs, and manages AI group managers.
	// The function also associates enemy spawn points with the closest tiles.
	// It spawns initial enemies and additional enemies based on the level generator configurations.
	UFUNCTION()
	void SpawnWorldInstances();

	UFUNCTION()
	void GenerationFinished();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void SpawnPrefabs();

	UFUNCTION()
	void SpawnLootCrates();

	UFUNCTION()
	void SpawnGroupManagers();
	
	UFUNCTION()
	void SpawnEnemies();
	
	TArray<ALevelGeneratorV2*> LevelGenerators;
	TArray<ATileBase*> AllSpawnedTiles;
	TMap<ALevelGeneratorV2*, TArray<ATileBase*>> LevelTiles;
	TMap<ALevelGeneratorV2*, TArray<FEnemyData>> LevelEnemyData;

	int Tier0LootCratesSpawned = 0;
	int Tier1LootCratesSpawned = 0;
	int Tier2LootCratesSpawned = 0;

	int LevelsGenerated = 0;
	int SpawnedEnemyCount = 0;
	FRandomStream FirstSeedStream;
};
