// Callum Stables, 2023


#include "DungeonGeneration/LevelGeneratorV2.h"

#include "EngineUtils.h"
#include "Components/BoxComponent.h"
#include "DungeonGeneration/GenerationEnums.h"
#include "DungeonGeneration/OffTileArea.h"
#include "DungeonGeneration/Prefabinator.h"
#include "Helpers/ArrayHelpers.h"
#include "Helpers/DungeonGenerationV2Helpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelGeneratorV2::ALevelGeneratorV2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALevelGeneratorV2::BeginPlay()
{
	Super::BeginPlay();

	//Return if safety checks fail
	if (!SafetyChecks()) return;

	TArray<FEnemyData*> EnemyDataTemp;
	if (EnemyDataTable) EnemyDataTable->GetAllRows<FEnemyData>(TEXT("GetAllRows in EnemyData"), EnemyDataTemp);
	else UE_LOG(LogTemp, Warning, TEXT("EnemyDataTable is not valid."));
	for (FEnemyData* Enemy : EnemyDataTemp)
	{
		if (Enemy) EnemyData.Add(*Enemy);
	}


	
	//Setup Random Seed
	if (bIsSeedRandom) SeedStream.GenerateNewSeed();
	else SeedStream.Initialize(Seed);

	//Generate Dungeon
	InitializeDungeon();
}

#if WITH_EDITOR
//Reset defaults on generation mode change
void ALevelGeneratorV2::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ALevelGeneratorV2, Mode)) {
		if (Mode != EGeneratorMode::Hybrid && Mode != EGeneratorMode::FullyRandom) {
			Length = 0;
			StepBackChance = 0;
			MaxCorridorsInRow = 0;
		}

		if (Mode != EGeneratorMode::Hybrid && Mode != EGeneratorMode::Fixed) {
			TargetLocation = FVector2D(0, 0);
		}

		if (Mode != EGeneratorMode::Hybrid) {
			MinDistanceFromTarget = 0;
		}

		if (!bOutBranching) {
			OutBranchingCount = 0;
		}
	}
}
#endif

void ALevelGeneratorV2::InitializeDungeon() {

	//Initalize and Reset the Dungeon
	CurrentLocation = FVector2D(0,0);
	TileLayout.Empty();

	//Setup Starting Tile
	StartingTile();

	//Setup TileLayout array based on offtile actors
	UpdateOffTilesFromWorldComponents();
	
	//Generate Dungeon Based On Mode
	switch (Mode) {
		case EGeneratorMode::FullyRandom:
			GenerateDungeonRandom(Length);
			break;
		case EGeneratorMode::Hybrid:
			GenerateDungeonHybrid(Length);
			break;
		case EGeneratorMode::Fixed:
			GenerateDungeonFixed();
			break;
	}

	if (bOutBranching) GenerateBranches();

	if (bHasKeyTile && bOutBranching) {
		if (!SpawnKeyTile()) {
			ResetAndRegenerate();
		} else {
			FTimerHandle AllTilesLoadedTimer;
			GetWorld()->GetTimerManager().SetTimer(AllTilesLoadedTimer, this, &ALevelGeneratorV2::SpawnDungeonTiles, 1.0f, false);
		}
	}
}

void ALevelGeneratorV2::StartingTile() {
	FTileInfo StartTile;
	StartTile.Type = ETileType::StartTile;
	*StartTile.Directions.Find(StartDirection) = true;
	TileLayout.Add(CurrentLocation, StartTile);

	// Generate the next tile from the starting tile
	FVector2D NextLocation = UDungeonGenerationV2Helpers::FindNextLocationSpecified(TileLayout, CurrentLocation, StartDirection);
	if (NextLocation != CurrentLocation) {
		FTileInfo NewTile;
		NewTile.Type = ETileType::GenericTile;
		*NewTile.Directions.Find(UDungeonGenerationV2Helpers::GetInverseCardinalDirection(StartDirection)) = true;
		TileLayout.Add(NextLocation, NewTile);

		// Update cardinal directions for the current (starting) and next tiles
		//UDungeonGenerationV2Helpers::UpdateTileDirections(TileLayout, CurrentLocation, NextLocation);
		CurrentLocation = NextLocation;
	}
}


void ALevelGeneratorV2::GenerateDungeonRandom(int32 Amount) {
	while (Amount > 0) {

		//Stepback or not based on probability
		FVector2D NextLocation;
		if (SeedStream.RandRange(0, 100) <= StepBackChance) 
			NextLocation = UDungeonGenerationV2Helpers::BacktrackerRandom(
			   TileLayout, CurrentLocation, SeedStream, MaxCorridorsInRow, TargetLocation, MinDistanceFromTarget, true);

		else NextLocation = UDungeonGenerationV2Helpers::FindNextLocationRandom(
				TileLayout, CurrentLocation, SeedStream, MaxCorridorsInRow, TargetLocation, MinDistanceFromTarget);

		if (NextLocation == FVector2D(0,0)) NextLocation = UDungeonGenerationV2Helpers::BacktrackerRandom(
				TileLayout, CurrentLocation, SeedStream, MaxCorridorsInRow, TargetLocation, MinDistanceFromTarget);

		// Check if the next location is different from the current location
		if (NextLocation != CurrentLocation) {
			
			// Generate a new tile at the next location
			FTileInfo NewTile;
			NewTile.Type = ETileType::GenericTile;
			TileLayout.Add(NextLocation, NewTile);

			// Update cardinal directions for the current and next tiles
			UDungeonGenerationV2Helpers::UpdateTileDirections(TileLayout, CurrentLocation, NextLocation);

			Amount--;
		} else {
			UE_LOG(LogTemp, Error, TEXT("No Valid Locations Found"));
			return;
		}

		CurrentLocation = NextLocation;
	}
}

void ALevelGeneratorV2::GenerateDungeonHybrid(int32 Amount) {
	GenerateDungeonRandom(Amount);
	
	if(UDungeonGenerationV2Helpers::CreatePathToTarget(TileLayout, TargetLocation)) {
		UE_LOG(LogTemp, Error, TEXT("Path Created"));
	} else {
		UE_LOG(LogTemp, Error, TEXT("Path Not Created"));
	}
}

void ALevelGeneratorV2::GenerateDungeonFixed() {
	if(UDungeonGenerationV2Helpers::CreatePathToTarget(TileLayout, TargetLocation)) {
		UE_LOG(LogTemp, Error, TEXT("Path Created"));
	} else {
		UE_LOG(LogTemp, Error, TEXT("Path Not Created"));
	}
}

bool ALevelGeneratorV2::GenerateBranches() {
	TArray<FVector2D> TileLocations = UDungeonGenerationV2Helpers::GetTileLayoutWithoutOffTiles(TileLayout, false);
	for (int32 i = 0; i < OutBranchingCount; ++i) {

		for (auto& Entry : TileLayout) {
			if (Entry.Value.Type != GenericTile) continue;
			
			CurrentLocation = Entry.Key;

			// Pick a random length
			const int32 BranchLength = SeedStream.RandRange(OutBranchingMin, OutBranchingMax);

			//Generate Branch && Set Key Tile
			if (UDungeonGenerationV2Helpers::BranchOutRandomPaths(TileLayout, CurrentLocation, SeedStream, MaxCorridorsInRow, BranchLength)) break;
		}
	}
	return true;
}

bool ALevelGeneratorV2::SpawnKeyTile() {
	for (auto& Entry : TileLayout) {
		if (Entry.Value.Type != GenericTile) continue;
		
		TArray<bool> CurrentTileMapDirections;
		Entry.Value.Directions.GenerateValueArray(CurrentTileMapDirections);
		
		if (UDungeonGenerationV2Helpers::GetConnectionTypeFromDirections(CurrentTileMapDirections) != Single) continue;
		
		Entry.Value.Type = KeyTile;
		return true;
	}
	return false;
}

void ALevelGeneratorV2::SpawnDungeonTiles() {
	if (SpawnableTiles.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("No spawnable tiles have been set."));
		return;
	}

	FAttachmentTransformRules TransformInfo (EAttachmentRule::KeepRelative,
EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative,true);

	TArray<UClass*> PreviousTiles;

	TMap<UClass*, int32> SpawnedTileCounts;
	
	for (const auto& Entry : TileLayout) {

		if (Entry.Value.Type == OffTile) continue;
		
		const FTileInfo& TileInfo = Entry.Value;
		FVector SpawnLocation(Entry.Key.X * TilesSize.X, Entry.Key.Y * TilesSize.Y, 0);

		// Find matching tile classes based on the TileInfo
		TArray<UClass*> MatchingTileClasses = UDungeonGenerationV2Helpers::FindMatchingTileClasses(TileInfo, SpawnableTiles);

		//Filter out tile classes that have reached their maximum instances
		for (int32 i = MatchingTileClasses.Num() - 1; i >= 0; --i) {
			UClass* CurrentTileClass = MatchingTileClasses[i];
			ATileBase* TileDefault = Cast<ATileBase>(CurrentTileClass->GetDefaultObject());
			int32& SpawnedCount = SpawnedTileCounts.FindOrAdd(CurrentTileClass);
    
			if (TileDefault->MaxInstances >= 1 && SpawnedCount >= TileDefault->MaxInstances) {
				MatchingTileClasses.RemoveAt(i);
			}
		}

		if (MatchingTileClasses.Num() > 0) {
			//Pick a random matching tile class using the seed
			int32 RandomIndex = SeedStream.RandRange(0, MatchingTileClasses.Num() - 1);
			UClass* TileClass = MatchingTileClasses[RandomIndex];
			
			//Prevent duplicate tiles in a row
			if ((PreviousTiles.Num() > 0 && PreviousTiles.Last() == TileClass)
				|| (PreviousTiles.Num() > 1 && PreviousTiles[PreviousTiles.Num() - 2] == TileClass)
				&& MatchingTileClasses.Num() > 1)
			{
				MatchingTileClasses.RemoveAt(RandomIndex);
				RandomIndex = SeedStream.RandRange(0, MatchingTileClasses.Num() - 1);
				if (RandomIndex < MatchingTileClasses.Num()) TileClass = MatchingTileClasses[RandomIndex];
			}

			// Update LastTwoTileClasses with the new TileClass
			if (PreviousTiles.Num() >= 2)PreviousTiles.RemoveAt(0);
			
			PreviousTiles.Add(TileClass);

			++SpawnedTileCounts[TileClass];

			ATileBase* NewTileClass = NewObject<ATileBase>(this,TileClass);
			
			FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);
			ATileBase* SpawnedTile = GetWorld()->SpawnActor<ATileBase>(NewTileClass->GetClass(), SpawnTransform);
			SpawnedTile->AttachToActor(this, TransformInfo);

			// Get the correct rotation using the TileInfo and the spawned tile
			FRotator CorrectRotation = UDungeonGenerationV2Helpers::GetTileRotation(TileInfo, SpawnedTile);
			SpawnedTile->SetActorRotation(CorrectRotation);
			SpawnedTiles.Add(SpawnedTile);

			//Spawn level instance
			if (!SpawnedTile->WorldAsset.IsNull()) {
				
				FTransform LevelTransform;
				SpawnLocation += GetActorLocation();
				
				LevelTransform = FTransform(CorrectRotation, SpawnLocation);

				FString LevelInstanceName = FString("Tile") + FString::FromInt(SpawnedTiles.Num());
				SpawnedTile->LevelInstance = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), SpawnedTile->WorldAsset, LevelTransform.GetLocation(), LevelTransform.Rotator(), SpawnedTile->bIsLevelLoaded);
				SpawnedTile->LevelInstance->bShouldBlockOnLoad = true;
				SpawnedTile->LevelInstance->SetShouldBeLoaded(true);
				SpawnedTile->LevelInstance->SetShouldBeVisible(true);
				SpawnedTile->SetupDelegate();
				SpawnedTile->OnLevelLoadedCustom.AddDynamic(this, &ALevelGeneratorV2::TileLoaded);
				MaxLoadedTilesCount++;
			}
			SpawnedTile->SetActorHiddenInGame(true);
			
		} else {
			UE_LOG(LogTemp, Error, TEXT("No matching tile class found for tile info."));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Ontilebasespawned broadcasted"));
	OnTileBaseSpawned.Broadcast(this, SpawnedTiles);
}

bool ALevelGeneratorV2::SafetyChecks() {
	
	// Check if there are any spawnable tiles
	if (SpawnableTiles.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("No Tiles To Spawn"));
		return false;
	}

	// Check if the TilesSize has valid dimensions
	if (TilesSize.X <= 0 || TilesSize.Y <= 0 || TilesSize.Z <= 0) {
		UE_LOG(LogTemp, Error, TEXT("Invalid TilesSize dimensions"));
		return false;
	}

	// Check if Length is valid
	if ((Mode == EGeneratorMode::Hybrid || Mode == EGeneratorMode::FullyRandom) && Length <= 0) {
		UE_LOG(LogTemp, Error, TEXT("Invalid Length value"));
		return false;
	}

	// Check if MaxCorridorsInRow is valid
	if ((Mode == EGeneratorMode::Hybrid || Mode == EGeneratorMode::FullyRandom) && MaxCorridorsInRow <= 2) {
		UE_LOG(LogTemp, Error, TEXT("Invalid MaxCorridorsInRow value"));
		return false;
	}

	// Check if MinDistanceFromTarget is valid
	if (Mode == EGeneratorMode::Hybrid && MinDistanceFromTarget < 0) {
		UE_LOG(LogTemp, Error, TEXT("Invalid MinDistanceFromTarget value"));
		return false;
	}
	
	return true;
}

void ALevelGeneratorV2::UpdateOffTilesFromWorldComponents() {
	TArray<AActor*> OffTileAreaActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOffTileArea::StaticClass(), OffTileAreaActors);

	if (OffTileAreaActors.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("No OffTileArea actors found in the world."));
		return;
	}

	OffTiles = UDungeonGenerationV2Helpers::UpdateOffTilesFromWorldComponents(OffTileAreaActors, TilesSize, GetActorLocation());

	if (OffTiles.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("No OffTiles found in the world."));
		return;
	}

	for (auto OffTile : OffTiles)
	{
		FTileInfo OffTileInfo;
		OffTileInfo.Type = ETileType::OffTile;
		TileLayout.Add(OffTile, OffTileInfo);
	}
}

void ALevelGeneratorV2::TileLoaded(ATileBase* Tile) {

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Tile Loaded"));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(LoadedTilesCount));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(MaxLoadedTilesCount));
	
	LoadedTilesCount++;
	
	//Check if all tiles are loaded
	if (LoadedTilesCount == MaxLoadedTilesCount) {
		OnAllLevelsGenerated.Broadcast();
	}

	Tile->OnLevelLoadedCustom.RemoveDynamic(this, &ALevelGeneratorV2::TileLoaded);
}

/*void ALevelGeneratorV2::AllTilesLoaded() {
	UE_LOG(LogTemp, Warning, TEXT("All tiles loaded"));
	UWorld* LevelWorld = GetWorld();

	if (!IsValid(LevelWorld)) return;

	// Collect required actors from the main world
	TArray<APrefabinator*> Prefabinators;
	TArray<AEnemySpawnPoint*> EnemySpawnPoints;
	TArray<AAIGroupManager*> AIGroupManagers;
	
	UDungeonGenerationV2Helpers::GetPrefabinatorsInLevel(LevelWorld, Prefabinators);
	UDungeonGenerationV2Helpers::GetEnemySpawnPointsInLevel(LevelWorld, EnemySpawnPoints);
	UDungeonGenerationV2Helpers::GetGroupManagersInLevel(LevelWorld, AIGroupManagers);
	

	// Spawn prefabinators
	for (APrefabinator* Prefabinator : Prefabinators)  Prefabinator->SpawnRandomPrefab(SeedStream);

	// Reset AIGroupManagers into the persistent world
	for (AAIGroupManager* AIGroupManager : AIGroupManagers) {
		if (!IsValid(AIGroupManager)) continue;
		
		AAIGroupManager* NewGroupManager = LevelWorld->SpawnActor<AAIGroupManager>(AIGroupManager->GetClass(), AIGroupManager->GetActorLocation(), AIGroupManager->GetActorRotation());
		NewGroupManager->SphereRadius = AIGroupManager->SphereRadius;
		NewGroupManager->SphereComponent->SetSphereRadius(AIGroupManager->SphereRadius);

		AIGroupManager->Destroy();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Enemy Spawn Points Num: %d"), EnemySpawnPoints.Num());

	// Register EnemySpawnPoints to the closest tile for initial spawnings
	for (AEnemySpawnPoint* EnemySpawnPoint : EnemySpawnPoints) {
		if (EnemySpawnPoint->bIsRegistered) continue;

		ATileBase* ClosestTile = nullptr;
		float ClosestDistance = FLT_MAX;
		float DistanceThreshold = 8000.0f; 

		for (ATileBase* Tile : SpawnedTiles) {
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
		}
	}

	// Spawn initial enemies
	if (EnemyData.Num() > 0) {
		for (auto Tile : SpawnedTiles) SpawnedEnemyCount += Tile->SpawnInitialEnemies(EnemyData, SeedStream);
	}

	// Spawn additional enemies randomly to reach max enemy count
	TArray<FEnemyData> EnemyDataCopy = EnemyData;
	for (auto EnemySpawnPoint : EnemySpawnPoints) {
		if (SpawnedEnemyCount >= MaxEnemyCount) break;
		if (EnemySpawnPoint->bHasSpawned) continue;
		if (EnemySpawnPoint->bSecondPass) continue;

		FEnemyData CurrentEnemyElement;
		UArrayHelpers::ShuffleArray(EnemyDataCopy, SeedStream);
		
		int RandomEnemyInt = SeedStream.RandRange(0, 100);

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
	}
}*/

void ALevelGeneratorV2::ResetAndRegenerate() {

	SeedStream.GenerateNewSeed();
	
	if (!SpawnedTiles.IsEmpty()) {
		for(auto Tile: SpawnedTiles) {
			Tile->Destroy();
		}
	}
	InitializeDungeon();
	
}

