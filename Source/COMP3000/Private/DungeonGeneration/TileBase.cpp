// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGeneration/TileBase.h"

#include "EngineUtils.h"
#include "DungeonGeneration/Prefabinator.h"
#include "Engine/LevelStreaming.h"
#include "Helpers/ArrayHelpers.h"
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

void ATileBase::SpawnPrefabinatorsRandomly() {
	if (LevelInstance == nullptr) return;
	
	/*TArray<APrefabinator*> Prefabinators;
	UWorld* LevelWorld = LevelInstance->GetWorld();
	
	for (TActorIterator<APrefabinator> It(LevelWorld); It; ++It)
	{
		Prefabinators.Add(*It);
	}
	
	UArrayHelpers::ShuffleArray(Prefabinators, SeedStream);

	for (APrefabinator* Prefabinator : Prefabinators)
	{
		Prefabinator->SpawnRandomPrefab(SeedStream);
	}*/
}

void ATileBase::SetupDelegate() {
	LevelInstance->OnLevelLoaded.AddDynamic(this, &ATileBase::SpawnPrefabinatorsRandomly);
}

