// Callum Stables, 2023


#include "DungeonGeneration/DungeonEnemyManager.h"

#include "Core/VarGameInstance.h"
#include "DungeonGeneration/Prefabinator.h"
#include "Helpers/ArrayHelpers.h"
#include "Helpers/DungeonGenerationV2Helpers.h"
#include "World/Collectables/LootCrate.h"

// Sets default values
ADungeonEnemyManager::ADungeonEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MaxEnemyCount = 60;
	Tier0LootCrateCount = 0;
	Tier1LootCrateCount = 0;
	Tier2LootCrateCount = 0;
}

// Called when the game starts or when spawned
void ADungeonEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(GetWorld())) return;
	
	UDungeonGenerationV2Helpers::GetLevelGeneratorsInWorld(GetWorld(), LevelGenerators);

	if (LevelGenerators.Num() == 0) return;
	
	for (auto& LevelGenerator : LevelGenerators)
	{
		if (!IsValid(LevelGenerator)) continue;

		LevelGenerator->OnTileBaseSpawned.AddDynamic(this, &ADungeonEnemyManager::SubAllTilesGenerated);
		LevelGenerator->OnAllLevelsGenerated.AddDynamic(this, &ADungeonEnemyManager::SubAllLevelsGenerated);
	}
	
}

void ADungeonEnemyManager::SubAllTilesGenerated(ALevelGeneratorV2* InGenerator, TArray<ATileBase*> InTileBases) {
	LevelTiles.Add(InGenerator, InTileBases);
	LevelEnemyData.Add(InGenerator, InGenerator->GetEnemyData());
	AllSpawnedTiles.Append(InTileBases);

	UE_LOG(LogTemp, Warning, TEXT("All tiles generated"));
	UE_LOG(LogTemp, Warning, TEXT("AllSpawnedTiles: %d"), AllSpawnedTiles.Num());
}

void ADungeonEnemyManager::SubAllLevelsGenerated() {
	LevelsGenerated++;

	if (IsValid(LevelGenerators[0]))  FirstSeedStream = LevelGenerators[0]->GetSeedStream();
	else FirstSeedStream.GenerateNewSeed();
	
	if (LevelsGenerated == LevelGenerators.Num()) {
		FTimerHandle AllTilesLoadedTimer;
		GetWorld()->GetTimerManager().SetTimer(AllTilesLoadedTimer, this, &ADungeonEnemyManager::SpawnWorldInstances, 3.0f, false);
	}
}

void ADungeonEnemyManager::SpawnWorldInstances() {
	UE_LOG(LogTemp, Warning, TEXT("All tiles loaded"));
	
	if (!IsValid(GetWorld())) return;
	
	SpawnPrefabs();
	SpawnGroupManagers();
	SpawnLootCrates();
	SpawnEnemies();

	FTimerHandle GenerationFinishedTimer;
	GetWorld()->GetTimerManager().SetTimer(GenerationFinishedTimer, this, &ADungeonEnemyManager::GenerationFinished, 3.0f, false);
}

void ADungeonEnemyManager::GenerationFinished() {
	UVarGameInstance* GameInstance = Cast<UVarGameInstance>(GetGameInstance());
	if (GameInstance) GameInstance->OnLevelGenerationFinished.Broadcast();
}

void ADungeonEnemyManager::SpawnPrefabs() {
	TArray<APrefabinator*> Prefabinators;
	UDungeonGenerationV2Helpers::GetPrefabinatorsInLevel(GetWorld(), Prefabinators);

	// Spawn prefabinators
	for (APrefabinator* Prefabinator : Prefabinators)  Prefabinator->SpawnRandomPrefab(FirstSeedStream);
}

void ADungeonEnemyManager::SpawnLootCrates() {
	TArray<ALootCrate*> LootCrates;
	UDungeonGenerationV2Helpers::GetLootCratesInWorld(GetWorld(), LootCrates);

	if (LootCrates.Num() == 0) return;

	UArrayHelpers::ShuffleArrayOfLootCrates(LootCrates, FirstSeedStream);
	
	for (ALootCrate* LootCrate : LootCrates) {
		if (!IsValid(LootCrate)) continue;
		if (LootCrate->bForcedSpawn) continue;

		if (Tier0LootCrateCount > Tier0LootCratesSpawned) {
			Tier0LootCratesSpawned++;
			LootCrate->EnableCrate(0);
			continue;
		}
		if (Tier1LootCrateCount > Tier1LootCratesSpawned) {
			Tier1LootCratesSpawned++;
			LootCrate->EnableCrate(1);
			continue;
		}
		if (Tier2LootCrateCount > Tier2LootCratesSpawned) {
			Tier2LootCratesSpawned++;
			LootCrate->EnableCrate(2);
		}
		break;
	}
}

void ADungeonEnemyManager::SpawnGroupManagers() {
	TArray<AAIGroupManager*> AIGroupManagers;
	UDungeonGenerationV2Helpers::GetGroupManagersInLevel(GetWorld(), AIGroupManagers);
	
	// Reset AIGroupManagers into the persistent world
	for (AAIGroupManager* AIGroupManager : AIGroupManagers) {
		if (!IsValid(AIGroupManager)) continue;
		
		AAIGroupManager* NewGroupManager = GetWorld()->SpawnActor<AAIGroupManager>(AIGroupManager->GetClass(), AIGroupManager->GetActorLocation(), AIGroupManager->GetActorRotation());
		NewGroupManager->SphereRadius = AIGroupManager->SphereRadius;
		NewGroupManager->SphereComponent->SetSphereRadius(AIGroupManager->SphereRadius);

		AIGroupManager->Destroy();
	}
}

void ADungeonEnemyManager::SpawnEnemies() {
	TArray<AEnemySpawnPoint*> EnemySpawnPoints;
	UDungeonGenerationV2Helpers::GetEnemySpawnPointsInLevel(GetWorld(), EnemySpawnPoints);
	
	for (AEnemySpawnPoint* EnemySpawnPoint : EnemySpawnPoints) {
		if (EnemySpawnPoint->bIsRegistered) continue;

		ATileBase* ClosestTile = nullptr;
		float ClosestDistance = FLT_MAX;
		float DistanceThreshold = 8000.0f; 

		for (ATileBase* Tile : AllSpawnedTiles) {
			if (!IsValid(Tile)) continue;

			UE_LOG(LogTemp, Warning, TEXT("Tile: %s"), *Tile->GetName());
			
			float SquaredDistance = FVector::DistSquared(Tile->GetActorLocation(), EnemySpawnPoint->GetActorLocation());
			if (SquaredDistance < ClosestDistance && SquaredDistance < DistanceThreshold * DistanceThreshold) {
				ClosestDistance = SquaredDistance;
				ClosestTile = Tile;
			}
		}

		if (IsValid(ClosestTile)) {
			EnemySpawnPoint->bIsRegistered = true;
			ClosestTile->TileEnemySpawnPoints.Add(EnemySpawnPoint);
			EnemySpawnPoint->AssociatedTB = ClosestTile;
		}
	}

	// Spawn initial enemies
	for (auto LevelGenerator : LevelGenerators) {
		if (!IsValid(LevelGenerator)) continue;
		if (LevelGenerator->GetEnemyData().Num() < 1) continue;
		
		for (auto Tile : AllSpawnedTiles) {

			//Associate the current tile with the enemy data
			for (const auto& KeyValuePair : LevelTiles)
			{
				ALevelGeneratorV2* Generator = KeyValuePair.Key;
				const TArray<ATileBase*>& Tiles = KeyValuePair.Value;

				if (Tiles.Contains(Tile))
				{
					UE_LOG(LogTemp, Warning, TEXT("tiles contains tile "));
					SpawnedEnemyCount += Tile->SpawnInitialEnemies(Generator->GetEnemyData(), FirstSeedStream);
					break;
				}
			}
		
		}

		// Spawn additional enemies randomly to reach max enemy count
		for (auto EnemySpawnPoint : EnemySpawnPoints) {
			if (SpawnedEnemyCount >= MaxEnemyCount) break;
			if (!IsValid(EnemySpawnPoint)) continue;
			if (EnemySpawnPoint->bHasSpawned) continue;
			if (EnemySpawnPoint->bSecondPass) continue;

			TArray<FEnemyData> EnemyDataCopy;

			ATileBase* CurrentTile = EnemySpawnPoint->AssociatedTB;
			if (!IsValid(EnemySpawnPoint->AssociatedTB)) continue;

			//Associate the current tile with the enemy data
			for (const auto& KeyValuePair : LevelTiles)
			{
				ALevelGeneratorV2* Generator = KeyValuePair.Key;
				const TArray<ATileBase*>& Tiles = KeyValuePair.Value;

				if (Tiles.Contains(CurrentTile))
				{
					EnemyDataCopy = Generator->GetEnemyData();
					break;
				}
			}

			if (EnemyDataCopy.Num() < 1) continue;
			
			FEnemyData CurrentEnemyElement;
			UArrayHelpers::ShuffleArray(EnemyDataCopy, FirstSeedStream);
		
			int RandomEnemyInt = FirstSeedStream.RandRange(0, 100);

			if (RandomEnemyInt >= 5) {
				for (int i = 0; i < EnemyDataCopy.Num(); i++) {
					if (EnemyDataCopy[i].EnemyType == EEnemyType::Minion) CurrentEnemyElement = EnemyDataCopy[i];
				}
			}
			else {
				for (int i = 0; i < EnemyDataCopy.Num(); i++) {
					if (EnemyDataCopy[i].EnemyType == EEnemyType::Elite) CurrentEnemyElement = EnemyDataCopy[i];
				}
			}

			EnemySpawnPoint->SpawnEnemy(CurrentEnemyElement);
			EnemySpawnPoint->bSecondPass = true;
			SpawnedEnemyCount++;
		}
	}
}