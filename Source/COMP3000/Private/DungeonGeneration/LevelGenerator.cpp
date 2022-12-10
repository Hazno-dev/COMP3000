// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGeneration/LevelGenerator.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TilesSize = FVector(3000, 3000, 3000);

	//Points to seek in generation
	SeekingPoints.Add(ECardinalPoints::North, FVector2D(1, 0));
	SeekingPoints.Add(ECardinalPoints::East, FVector2D(0, 1));
	SeekingPoints.Add(ECardinalPoints::South, FVector2D(-1, 0));
	SeekingPoints.Add(ECardinalPoints::West, FVector2D(0, -1));

}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	TileLayoutGeneration();
	SpawnTileLayout();
	
	//SpawnTile(StartTile, FVector(0,0,0));
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Generate dungeon layout
void ALevelGenerator::TileLayoutGeneration()
{
	//Start Tile
	FTileInfo BuildInfo;
	BuildInfo.Type = ETileType::StartTile;
	BuildInfo.Directions = StartTile->GetDefaultObject<ATileBase>()->TileConnections;
	TileLayout.Add(FVector2D(0,0), BuildInfo);

	PreviousLocation = FVector2D(0,0);
	CurrentLocation = FVector2D(0,0);
	TempDirect = StartDirection;
	
	//Loop as many times as ValueAmount (amount of tiles to generate)
	for (int i = 0; i < ValueAmount; i++)
	{
		FTileInfo MakerInfo;
		MakerInfo.Type = ETileType::GenericTile;
		
		if (TileLayout.Num() == 1) CurrentLocation = FindNextRandomLocation(PreviousLocation, StartDirection);
		else CurrentLocation = FindNextRandomLocation(PreviousLocation, Random);
		TileLayout.Add(CurrentLocation, MakerInfo);
		TileConnector(PreviousLocation, CurrentLocation, TempDirect);
		PreviousLocation = CurrentLocation;
	}
	//CurrentLocation = FindNextRandomLocation(PreviousLocation, EndTile->GetDefaultObject<ATileBase>()->TileDirection);
}

//Iterate through TileLayout and spawn each item.
void ALevelGenerator::SpawnTileLayout()
{
	for (auto tile : TileLayout)
	{
		switch (tile.Value.Type)
		{
			case ETileType::StartTile:
				SpawnTile(StartTile, GetTileWorldspace(tile.Key));
				break;
			case ETileType::GenericTile:
				std::vector<UClass*> ValidTiles;
				for (auto CurrentGenericTile : GenericTiles)
				{
					//Loop through current tile, compare against list of generics. If both directions are equal, add to list of valid spawns for a tile.
					TArray<bool> CurrentTileMapDirections;
					TArray<bool> CurrentTileListDirections;
					tile.Value.Directions.GenerateValueArray(CurrentTileMapDirections);
					CurrentGenericTile->GetDefaultObject<ATileBase>()->TileConnections.GenerateValueArray(CurrentTileListDirections);
					for (int i = 0; i < 4; i++)
					{
						if (CurrentTileMapDirections[i] != CurrentTileListDirections[i]) break;
						if (i == 3) ValidTiles.push_back(CurrentGenericTile);
					}
				}
				if (ValidTiles.size() > 0) SpawnTile(ValidTiles[FMath::RandRange(0, ValidTiles.size()-1)], GetTileWorldspace(tile.Key));
				break;
			
		}
	}
}

//Spawn a Tile
void ALevelGenerator::SpawnTile(UClass* TileClass, const FVector& position)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<ATileBase>(TileClass, position, FRotator::ZeroRotator, SpawnInfo);
}

//Get a tiles vector2d, convert into worldspace location.
FVector ALevelGenerator::GetTileWorldspace(FVector2D InVec)
{
	return FVector(InVec.X * TilesSize.X, InVec.Y * TilesSize.Y, 0);
}

FVector2D ALevelGenerator::FindNextRandomLocation(FVector2D InVec, TEnumAsByte<ECardinalPoints> WantedDirection)
{
	//Locations to seek
	if (WantedDirection == ECardinalPoints::Random)
	{
		std::vector<FVector2D> ValidLocations;
		std::vector<TEnumAsByte<ECardinalPoints>> ValidDirects;
		for (auto CurrentSeek : SeekingPoints) {
			if (TileLayout.Find(InVec + CurrentSeek.Value) == nullptr && CurrentSeek.Key != OffDirection) {
				ValidLocations.push_back(InVec + CurrentSeek.Value);
				ValidDirects.push_back(CurrentSeek.Key);
			}
		}
		int rand = FMath::RandRange(0, ValidLocations.size() - 1);
		if (ValidLocations.size() > 1) {
			TempDirect = ValidDirects[rand];
			return ValidLocations[rand];
		}
		if (ValidLocations.size() > 0) {
			TempDirect = ValidDirects[0];
			return ValidLocations[0];
		}
		return InVec;
	}
	if (TileLayout.Find(InVec + SeekingPoints.FindRef(WantedDirection)) == nullptr)
	{
		TempDirect = WantedDirection;
		return InVec + SeekingPoints.FindRef(WantedDirection);
	}
	return InVec;
}

void ALevelGenerator::TileConnector(FVector2D FirstTileVec, FVector2D SecondTileVec, TEnumAsByte<ECardinalPoints> FirstCardinal)
{
	//UE_LOG(LogTemp, Warning, TEXT("GripState: %i"), FirstCardinal);
	TEnumAsByte<ECardinalPoints> DummyCard;
	switch (FirstCardinal)
	{
		case North: DummyCard = South; break;
		case South: DummyCard = North; break;
		case East: DummyCard = West; break;
		case West: DummyCard = East; break;
	}
	SetConnection(FirstTileVec, FirstCardinal, true);
	SetConnection(SecondTileVec, DummyCard, true);
}

void ALevelGenerator::SetConnection(FVector2D InTileVec, TEnumAsByte<ECardinalPoints> ToCardinal, bool enable)
{
	if (TileLayout.Find(InTileVec) != nullptr) TileLayout.Find(InTileVec)->Directions.Add(ToCardinal, enable);
}

