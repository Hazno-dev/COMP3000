// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGeneration/TileBase.h"

#include "EngineUtils.h"
#include "AI/EnemySpawnPoint.h"
#include "DungeonGeneration/Prefabinator.h"
#include "Engine/LevelStreaming.h"
#include "Helpers/ArrayHelpers.h"
#include "Helpers/DungeonGenerationV2Helpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATileBase::ATileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TileConnections.Add(ECardinalPoints::North, false);
	TileConnections.Add(ECardinalPoints::East, false);
	TileConnections.Add(ECardinalPoints::South, false);
	TileConnections.Add(ECardinalPoints::West, false);
	TileConnections.Add(ECardinalPoints::Random, false);
	bMultiDirectional = true;

	MaxInstances = -1;

	MinMinionsToSpawn = 10;
	MinEliteToSpawn = 2;
	MaxMinionsToSpawn = 15;
	MaxEliteToSpawn = 3;
}

// Called when the game starts or when spawned
void ATileBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileBase::TileLoaded() {
	OnLevelLoadedCustom.Broadcast(this);

	LevelInstance->OnLevelLoaded.RemoveDynamic(this, &ATileBase::TileLoaded);
}

void ATileBase::SetupDelegate() {
	LevelInstance->OnLevelLoaded.AddDynamic(this, &ATileBase::TileLoaded);
}

int32 ATileBase::SpawnInitialEnemies(TArray<FEnemyData> EnemyData, FRandomStream SeedStream) {
	if (!IsValid(GetWorld())) return 0;
	if (TileEnemySpawnPoints.Num() == 0) return 0;
	
	int32 MinionsSpawned = 0;
	int32 EliteSpawned = 0;

	//Use a roulette wheel slection algorithm to spawn enemies with weighting.
	for (auto SpawnPoint : TileEnemySpawnPoints) {
		if (!IsValid(SpawnPoint)) continue;
		if (SpawnPoint->EnemyType == Minion && MinionsSpawned >= MinMinionsToSpawn) continue;
		if (SpawnPoint->EnemyType == Elite && EliteSpawned >= MinEliteToSpawn) continue;
		
		TArray<FEnemyData> EnemyDataCopy = EnemyData;
		UArrayHelpers::ShuffleArray(EnemyDataCopy, SeedStream);

		// Filter the enemy data by the spawn point's enemy type
		TArray<FEnemyData> FilteredEnemyData = EnemyDataCopy.FilterByPredicate([&](const FEnemyData& Data) {
			return Data.EnemyType == SpawnPoint->EnemyType;
		});

		//Calculate the total weight for the filtered enemy data
		float TotalWeight = 0.0f;
		for (const FEnemyData& Data : FilteredEnemyData) {
			TotalWeight += Data.SpawnWeight;
		}
		
		float RandomWeight = SeedStream.FRandRange(0.0f, TotalWeight);

		// Iterate over the filtered enemy data and choose an enemy based on the weight
		for (FEnemyData& Data : FilteredEnemyData) {
			RandomWeight -= Data.SpawnWeight;

			if (RandomWeight <= 0.0f) {
				// Spawn the chosen enemy
				SpawnPoint->SpawnEnemy(Data);

				// Update spawn counters based on enemy type
				if (Data.EnemyType == Minion) {
					MinionsSpawned++;
				} else if (Data.EnemyType == Elite) {
					EliteSpawned++;
				}

				break;
			}
		}
	}
	
	return MinionsSpawned + EliteSpawned;
	
}

