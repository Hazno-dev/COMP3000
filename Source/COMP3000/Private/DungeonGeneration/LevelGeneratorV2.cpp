// Callum Stables, 2023


#include "DungeonGeneration/LevelGeneratorV2.h"

#include "DungeonGeneration/GenerationEnums.h"
#include "Helpers/DungeonGenerationV2Helpers.h"

// Sets default values
ALevelGeneratorV2::ALevelGeneratorV2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelGeneratorV2::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnableTiles.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("No Tiles To Spawn"));
		return;
	}
	
	InitializeDungeon();
}

// Called every frame
void ALevelGeneratorV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelGeneratorV2::InitializeDungeon() {

	//Initalize and Reset the Dungeon
	SeedStream.Initialize(Seed);
	CurrentLocation = FVector2D(0,0);
	TileLayout.Empty();

	//Setup Starting Tile
	StartingTile();

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
		TileLayout.Add(NextLocation, NewTile);

		// Update cardinal directions for the current (starting) and next tiles
		UDungeonGenerationV2Helpers::UpdateTileDirections(TileLayout, CurrentLocation, NextLocation);
		CurrentLocation = NextLocation;
	}
}


void ALevelGeneratorV2::GenerateDungeonRandom(int32 Amount) {
	while (Amount > 0) {
		FVector2D NextLocation = UDungeonGenerationV2Helpers::FindNextLocationRandom(
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
}

void ALevelGeneratorV2::GenerateDungeonFixed() {
}

void ALevelGeneratorV2::SpawnDungeonTiles() {
	if (SpawnableTiles.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("No spawnable tiles have been set."));
		return;
	}

	for (const auto& Entry : TileLayout) {
		FVector2D TilePosition = Entry.Key;
		const FTileInfo& TileInfo = Entry.Value;

		// Convert the FVector2D position to a 3D position using the TilesSize
		FVector SpawnLocation(TilePosition.X * TilesSize.X, TilePosition.Y * TilesSize.Y, 0);

		// Find matching tile classes based on the TileInfo
		TArray<UClass*> MatchingTileClasses = UDungeonGenerationV2Helpers::FindMatchingTileClasses(TileInfo, SpawnableTiles);

		if (MatchingTileClasses.Num() > 0) {
			// Pick a random matching tile class using the seed
			int32 RandomIndex = SeedStream.RandRange(0, MatchingTileClasses.Num() - 1);
			UClass* TileClass = MatchingTileClasses[RandomIndex];

			// Spawn the tile at the determined location without rotation
			FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);
			ATileBase* SpawnedTile = GetWorld()->SpawnActor<ATileBase>(TileClass, SpawnTransform);

			// Get the correct rotation using the TileInfo and the spawned tile
			FRotator CorrectRotation = UDungeonGenerationV2Helpers::GetTileRotation(TileInfo, SpawnedTile);

			// Set the spawned tile's rotation
			SpawnedTile->SetActorRotation(CorrectRotation);

			FAttachmentTransformRules TransformInfo (EAttachmentRule::KeepRelative,
	EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative,true);
			
			SpawnedTile->AttachToActor(this, TransformInfo);
		} else {
			UE_LOG(LogTemp, Error, TEXT("No matching tile class found for tile info."));
		}
	}
}



