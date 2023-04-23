// Callum Stables, 2023


#include "DungeonGeneration/PrefabinatorDebug.h"

#include "EngineUtils.h"
#include "DungeonGeneration/Prefabinator.h"
#include "Helpers/ArrayHelpers.h"

// Sets default values
APrefabinatorDebug::APrefabinatorDebug()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APrefabinatorDebug::BeginPlay()
{
	Super::BeginPlay();
	
	SeedStream.GenerateNewSeed();
	
	if (bSpawnAllPrefabs) SpawnAllPrefabs();
}

// Called every frame
void APrefabinatorDebug::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrefabinatorDebug::SpawnAllPrefabs() {
	TArray<APrefabinator*> Prefabinators;
	UWorld* LevelWorld = GetWorld();
	
	for (TActorIterator<APrefabinator> It(LevelWorld); It; ++It)
	{
		Prefabinators.Add(*It);
	}
	
	UArrayHelpers::ShuffleArray(Prefabinators, SeedStream);

	for (APrefabinator* Prefabinator : Prefabinators)
	{
		Prefabinator->SpawnRandomPrefab(SeedStream);
	}
}
