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

	//Setup Random Seed
	if (bIsSeedRandom) SeedStream.GenerateNewSeed();
	else SeedStream.Initialize(Seed);

	//Generate Dungeon
	InitializeDungeon();
}

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

	SpawnDungeonTiles();
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
				TileLayout, CurrentLocation, SeedStream, MaxCorridorsInRow, TargetLocation, MinDistanceFromTarget);
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

void ALevelGeneratorV2::GenerateBranches() {
	TArray<FVector2D> TileLocations = UDungeonGenerationV2Helpers::GetTileLayoutWithoutOffTiles(TileLayout, false);
	for (int32 i = 0; i < OutBranchingCount; ++i) {
		// Pick a random tile location
		const int32 RandomIndex = SeedStream.RandRange(0, TileLocations.Num() - 1);
		CurrentLocation = TileLocations[RandomIndex];

		// Pick a random length
		const int32 BranchLength = SeedStream.RandRange(OutBranchingMin, OutBranchingMax);

		//Generate Branch
		UDungeonGenerationV2Helpers::BranchOutRandomPaths(TileLayout, CurrentLocation, SeedStream, MaxCorridorsInRow, BranchLength);
	}
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
			
			FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);
			ATileBase* SpawnedTile = GetWorld()->SpawnActor<ATileBase>(TileClass, SpawnTransform);
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
				SpawnedTile->LevelInstance->SetShouldBeLoaded(true);
				SpawnedTile->LevelInstance->SetShouldBeVisible(true);
				SpawnedTile->SetupDelegate();
				SpawnedTile->LevelInstance->OnLevelLoaded.AddDynamic(this, &ALevelGeneratorV2::TileLoaded);
				MaxLoadedTilesCount++;
			}
			SpawnedTile->SetActorHiddenInGame(true);
			
		} else {
			UE_LOG(LogTemp, Error, TEXT("No matching tile class found for tile info."));
		}
	}
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

void ALevelGeneratorV2::TileLoaded() {

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Tile Loaded"));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(LoadedTilesCount));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(MaxLoadedTilesCount));
	
	LoadedTilesCount++;
	if (LoadedTilesCount == MaxLoadedTilesCount) {
		
		AllTilesLoaded();
	}
}

void ALevelGeneratorV2::AllTilesLoaded() {
	UE_LOG(LogTemp, Warning, TEXT("All tiles loaded"));
	TArray<APrefabinator*> Prefabinators;
	UWorld* LevelWorld = GetWorld();

	if (!IsValid(LevelWorld)) return;

	// Function to collect Prefabinators from a given world
	auto CollectPrefabinators = [](UWorld* InWorld, TArray<APrefabinator*>& OutPrefabinators) {
		for (TActorIterator<APrefabinator> It(InWorld); It; ++It)
		{
			OutPrefabinators.Add(*It);
		}
	};
		
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(Prefabinators.Num()));

	// Collect Prefabinators from the main world
	CollectPrefabinators(LevelWorld, Prefabinators);

		
	// Collect Prefabinators from level instances
	for (ULevelStreaming* LevelStreaming : LevelWorld->GetStreamingLevels())
	{
		if (LevelStreaming && LevelStreaming->IsLevelLoaded())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Level Loaded"));
			UWorld* StreamingWorld = LevelStreaming->GetWorld();
			if (IsValid(StreamingWorld))
			{
				CollectPrefabinators(StreamingWorld, Prefabinators);
			}
		}
	}

	for (APrefabinator* Prefabinator : Prefabinators)
	{
		Prefabinator->SpawnRandomPrefab(SeedStream);
	}
}

void ALevelGeneratorV2::ResetAndRegenerate() {

	if (!SpawnedTiles.IsEmpty()) {
		for(auto Tile: SpawnedTiles) {
			Tile->Destroy();
		}
	}
	InitializeDungeon();
	
}

