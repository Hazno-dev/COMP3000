// Callum Stables, 2023


#include "Helpers/DungeonGenerationV2Helpers.h"

#include "EngineUtils.h"
#include "Components/BoxComponent.h"
#include "DungeonGeneration/LevelGeneratorV2.h"
#include "DungeonGeneration/OffTileArea.h"
#include "World/PSpawnPoint.h"
#include "World/Collectables/LootCrate.h"

FVector2D UDungeonGenerationV2Helpers::FindNextLocationRandom(TMap<FVector2D, FTileInfo>& TileLayout,
                                                              FVector2D& CurrentLocation, FRandomStream& Seed, int32 MaxCorridorsInRow, FVector2D TargetLocation, float MinDistanceFromTarget, bool bUseFisherYates) {

	// Find valid locations and directions
	TArray<FVector2D> ValidLocations;
	TArray<ECardinalPoints> ValidDirections;

	// Loop through potential locations and check their validity
	for (int32 i = 0; i < 4; ++i) {
		ECardinalPoints CardinalPoint = static_cast<ECardinalPoints>(i);
		FVector2D Direction = CardinalDirections[i];
		FVector2D CandidateLocation = CurrentLocation + Direction;

		if (!TileLayout.Contains(CandidateLocation) && FVector2D::Distance(CandidateLocation, TargetLocation) >= MinDistanceFromTarget) {
			ValidLocations.Add(CandidateLocation);
			ValidDirections.Add(CardinalPoint);
		}
	}

	if (bUseFisherYates) {
		// Shuffle the valid locations and directions using Fisher-Yates shuffle
		for (int32 i = ValidLocations.Num() - 1; i > 0; --i) {
			int32 j = Seed.RandRange(0, i);
			ValidLocations.Swap(i, j);
			ValidDirections.Swap(i, j);
		}
	} else {
		// Sort the valid locations based on the sum of distances to neighboring tiles
		ValidLocations.Sort([&](const FVector2D& A, const FVector2D& B) {
			//Small amount of noise to prevent all tiles just being sorted by distance
			float RandomFactorA = Seed.FRandRange(0.0f, 2.0f);
			float RandomFactorB = Seed.FRandRange(0.0f, 2.0f);
			return (CalculateNeighboringTilesDistanceSum(TileLayout, A) + RandomFactorA) < (CalculateNeighboringTilesDistanceSum(TileLayout, B) + RandomFactorB);
		});
	}

	// Iterate over shuffled or sorted valid locations
	for (int32 i = 0; i < ValidLocations.Num(); ++i) {
		FVector2D SelectedLocation = ValidLocations[i];

		// Check if the selected location would form a corridor
		if (!IsNextLocationCorridor(TileLayout, SelectedLocation, MaxCorridorsInRow)) {
			return SelectedLocation;
		}
	}

	// If no valid location is found
	return FVector2D(0, 0);
}




FVector2D UDungeonGenerationV2Helpers::FindNextLocationSpecified(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D& CurrentLocation, ECardinalPoints Direction) {
	FVector2D NextLocation = CurrentLocation + CardinalPointToFVector2D(Direction);

	// Check if the next location is not occupied
	if (!TileLayout.Contains(NextLocation)) {
		return NextLocation;
	}

	// If the next location is already occupied, return the current location
	return CurrentLocation;
}


FVector2D UDungeonGenerationV2Helpers::BacktrackerRandom(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D& CurrentLocation, FRandomStream& Seed,
	int32 MaxCorridorsInRow, FVector2D TargetLocation, float MinDistanceFromTarget, bool bSkipFirst) {

	//if (TileLayout.Find(CurrentLocation)->Type == StartTile) return CurrentLocation;
	
	TArray<FVector2D> TileLocations = GetTileLayoutWithoutOffTiles(TileLayout);
	int counter = GetTileCountWithoutOffTiles(TileLayout) - 1;
	bool foundValidLocation = false;

	if (bSkipFirst && counter > 3) {
		counter--;
	}

	while (counter > 0 && !foundValidLocation) {
		CurrentLocation = TileLocations[counter];
		
		FVector2D NextLocation = FindNextLocationRandom(TileLayout, CurrentLocation, Seed, MaxCorridorsInRow, TargetLocation, MinDistanceFromTarget);

		// If the next location is not the current location, we have found a valid location.
		if (CurrentLocation != NextLocation && NextLocation != FVector2D(0, 0)) {
			foundValidLocation = true;
			return NextLocation;
		}
		counter--;
	}

	return CurrentLocation;
}

bool UDungeonGenerationV2Helpers::BranchOutRandomPaths(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D& CurrentLocation, FRandomStream& Seed,
	int32 MaxCorridorsInRow, int32 BranchLength) {
	
	for (int32 j = 0; j < BranchLength; ++j) {
		FVector2D NextLocation = FindNextLocationRandom(TileLayout, CurrentLocation, Seed, MaxCorridorsInRow, FVector2D::ZeroVector, 0, false);

		if (NextLocation != CurrentLocation && NextLocation != FVector2D(0, 0)) {
			FTileInfo NewTileInfo;
			NewTileInfo.Type = GenericTile;
			TileLayout.Add(NextLocation, NewTileInfo);
			
			UpdateTileDirections(TileLayout, CurrentLocation, NextLocation);
			CurrentLocation = NextLocation;
		} else if (j == 0) return false;
	}
	
	return true;
}


TArray<UClass*> UDungeonGenerationV2Helpers::FindMatchingTileClasses(const FTileInfo& TileInfo, TArray<UClass*>& SpawnableTiles) {
	TArray<UClass*> MatchingTileClasses;
	for (UClass* TileClass : SpawnableTiles) {
		ATileBase* TileBase = Cast<ATileBase>(TileClass->GetDefaultObject());

		if (!TileBase) {
			continue;
		}

		if (DoTileInfoAndTileBaseMatch(TileInfo, TileBase)) {
			MatchingTileClasses.Add(TileClass);
		} 
		
	}
	return MatchingTileClasses;
}


bool UDungeonGenerationV2Helpers::DoTileInfoAndTileBaseMatch(const FTileInfo& TileInfo, const ATileBase* TileBase) {
	// Check if the TileType matches
	if (TileInfo.Type != TileBase->TileType) {
		return false;
	}

	// Check if the TileConnections match
	TArray<bool> CurrentTileMapDirections;
	TileInfo.Directions.GenerateValueArray(CurrentTileMapDirections);

	EConnectionType TileInfoConnectionType = GetConnectionTypeFromDirections(CurrentTileMapDirections);

	if (TileBase->bMultiDirectional) {
		return TileBase->ConnectionType == TileInfoConnectionType;
	}

	TArray<bool> CurrentTileListDirections;
	TileBase->TileConnections.GenerateValueArray(CurrentTileListDirections);

	for (int i = 0; i < 4; i++) {
		if (CurrentTileMapDirections[i] != CurrentTileListDirections[i]) {
			UE_LOG(LogTemp, Warning, TEXT("Tile connection does not match for direction index %d"), i);
			return false;
		}
	}

	return true;
}



FRotator UDungeonGenerationV2Helpers::GetTileRotation(const FTileInfo& TileInfo, const ATileBase* TileBase) {
	// Determine the rotation based on the TileInfo.Directions and bMultiDirectional property
	float Yaw = 0.0f;

	if (TileBase->bMultiDirectional) {
		// If the tile is multi-directional, we need to find the rotation based on the directions and connection type
		TArray<bool> CurrentTileMapDirections;
		TileInfo.Directions.GenerateValueArray(CurrentTileMapDirections);

		//Fourway
		if (CurrentTileMapDirections[0] && CurrentTileMapDirections[1] && CurrentTileMapDirections[2] && CurrentTileMapDirections[3]) Yaw = 0;

		//Threeway
		if (CurrentTileMapDirections[0] && CurrentTileMapDirections[1] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[2]) Yaw =  0;
		if (CurrentTileMapDirections[0] && CurrentTileMapDirections[1] && CurrentTileMapDirections[2] && !CurrentTileMapDirections[3]) Yaw =  90;
		if (CurrentTileMapDirections[1] && CurrentTileMapDirections[2] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[0]) Yaw =  180;
		if (CurrentTileMapDirections[0] && CurrentTileMapDirections[2] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[1]) Yaw =  270;
		
		//Corner
		if (CurrentTileMapDirections[0] && CurrentTileMapDirections[1] && !CurrentTileMapDirections[2] && !CurrentTileMapDirections[3]) Yaw =  0;
		if (CurrentTileMapDirections[1] && CurrentTileMapDirections[2] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[3]) Yaw =  90;
		if (CurrentTileMapDirections[2] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[0]) Yaw =  180;
		if (CurrentTileMapDirections[3] && CurrentTileMapDirections[0] && !CurrentTileMapDirections[2] && !CurrentTileMapDirections[1]) Yaw =  270;

		//Line
		if (CurrentTileMapDirections[0] && CurrentTileMapDirections[2] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[3]) Yaw =  0;
		if (CurrentTileMapDirections[1] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[2]) Yaw =  90;

		//Single

		if (CurrentTileMapDirections[0] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[2] && !CurrentTileMapDirections[3]) Yaw =  0;
		if (CurrentTileMapDirections[1] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[2] && !CurrentTileMapDirections[3]) Yaw =  90;
		if (CurrentTileMapDirections[2] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[3]) Yaw =  180;
		if (CurrentTileMapDirections[3] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[2]) Yaw =  270;
		
	}
	return FRotator(0.0f, Yaw, 0.0f);
}

ATileBase* UDungeonGenerationV2Helpers::SpawnTile(UClass* TileToSpawn, FVector2D Location, ECardinalPoints Direction,
                                                  UWorld* World) {
	return nullptr;
}

bool UDungeonGenerationV2Helpers::GetPrefabinatorsInLevel(UWorld* InWorld,
	TArray<APrefabinator*>& OutPrefabinators) {
	int NumPrefabinators = 0;
	for (TActorIterator<APrefabinator> It(InWorld); It; ++It)
	{
		OutPrefabinators.Add(*It);
		NumPrefabinators++;
	}
	return NumPrefabinators > 0;
}

bool UDungeonGenerationV2Helpers::GetEnemySpawnPointsInLevel(UWorld* InWorld,
	TArray<AEnemySpawnPoint*>& OutEnemySpawnPoints) {
	int NumEnemySpawnPoints = 0;
	for (TActorIterator<AEnemySpawnPoint> It(InWorld, AEnemySpawnPoint::StaticClass()); It; ++It)
	{
		OutEnemySpawnPoints.Add(*It);
		NumEnemySpawnPoints++;
	}
	return NumEnemySpawnPoints > 0;
}

bool UDungeonGenerationV2Helpers::GetGroupManagersInLevel(UWorld* InWorld, TArray<AAIGroupManager*>& OutGroupManagers) {
	int NumGroupManagers = 0;
	for (TActorIterator<AAIGroupManager> It(InWorld, AAIGroupManager::StaticClass()); It; ++It)
	{
		OutGroupManagers.Add(*It);
		NumGroupManagers++;
	}
	return NumGroupManagers > 0;
}

bool UDungeonGenerationV2Helpers::GetSpawnPointsInWorld(UWorld* InWorld, TArray<APSpawnPoint*>& OutSpawnPoints) {
	int NumSpawnPoints = 0;
	for (TActorIterator<APSpawnPoint> It(InWorld, APSpawnPoint::StaticClass()); It; ++It)
	{
		OutSpawnPoints.Add(*It);
		NumSpawnPoints++;
	}
	return NumSpawnPoints > 0;
}

bool UDungeonGenerationV2Helpers::GetLevelGeneratorsInWorld(UWorld* InWorld,
	TArray<ALevelGeneratorV2*>& OutLevelGenerators) {
	int NumLevelGenerators = 0;
	for (TActorIterator<ALevelGeneratorV2> It(InWorld, ALevelGeneratorV2::StaticClass()); It; ++It)
	{
		OutLevelGenerators.Add(*It);
		NumLevelGenerators++;
	}
	return NumLevelGenerators > 0;
}

bool UDungeonGenerationV2Helpers::GetLootCratesInWorld(UWorld* InWorld, TArray<ALootCrate*>& OutLootCrates) {
	int NumLootCrates = 0;
	for (TActorIterator<ALootCrate> It(InWorld, ALootCrate::StaticClass()); It; ++It)
	{
		OutLootCrates.Add(*It);
		NumLootCrates++;
	}
	return NumLootCrates > 0;
}


bool UDungeonGenerationV2Helpers::IsNextLocationCorridor(const TMap<FVector2D, FTileInfo>& TileLayout,
                                                         FVector2D NextLocation, int32 MaxCorridorsInRow) {

	int32 CorridorCount = 0;

	// Iterate through all directions
	for (const FVector2D& Direction : CardinalDirections) {
		FVector2D AdjacentLocation = NextLocation + Direction;
		FTileInfo AdjacentTileInfo = TileLayout.FindRef(AdjacentLocation);

		// If the tile is a starting/off tile, ignore it
		if (AdjacentTileInfo.Type == StartTile || AdjacentTileInfo.Type == OffTile) continue;
		
		
		// If the adjacent tile is connected in the direction of the current tile, increment the corridor count
		FVector2D ReverseDirection = -Direction;
		ECardinalPoints ReverseCardinalPoint = FVector2DToCardinalPoint(ReverseDirection);
		if (AdjacentTileInfo.Directions.FindRef(ReverseCardinalPoint)) {
			CorridorCount++;
		}
	}

	// If the corridor count is greater than or equal to the maximum allowed, return true (it's a corridor)
	if (CorridorCount >= MaxCorridorsInRow) return true;

	// If the corridor count is less than the maximum allowed, return false (it's not a corridor)
	return false;
}

float UDungeonGenerationV2Helpers::CalculateNeighboringTilesDistanceSum(const TMap<FVector2D, FTileInfo>& TileLayout, const FVector2D& Location) {
	float DistanceSum = 0;

	for (int32 i = 0; i < 4; ++i) {
		FVector2D Direction = CardinalDirections[i];
		FVector2D NeighborLocation = Location + Direction;

		if (TileLayout.Contains(NeighborLocation)) {
			DistanceSum += FVector2D::Distance(Location, NeighborLocation);
		}
	}

	return DistanceSum;
}

TArray<FVector2D> UDungeonGenerationV2Helpers::UpdateOffTilesFromWorldComponents(TArray<AActor*> OffTileAreaActors, FVector TilesSize, FVector LevelGeneratorPosition) {
    TArray<FVector2D> OffTiles = TArray<FVector2D>();

    // Loop through all the actors in OffTileAreaActors array
    for (AActor* Actor : OffTileAreaActors) {
    	
        AOffTileArea* OffTileArea = Cast<AOffTileArea>(Actor);
        if (OffTileArea) {
            // Calculate the position of the OffTileArea actor relative to the LevelGenerator
            FVector RelativePosition = OffTileArea->GetActorLocation() - LevelGeneratorPosition;
            UBoxComponent* BoxComponent = OffTileArea->GetBoxComponent();

            // Check if the BoxComponent exists
            if (BoxComponent) {
                // Calculate the number of tiles in both X and Y dimensions based on the BoxComponent size
                FVector BoxExtent = BoxComponent->GetScaledBoxExtent();
                FIntPoint TileCount(FMath::RoundToInt(BoxExtent.X * 2 / TilesSize.X), FMath::RoundToInt(BoxExtent.Y * 2 / TilesSize.Y));

                // Skip if the box size is too small
                if (TileCount.X <= 0 || TileCount.Y <= 0) {
                    continue;
                }

                // Calculate the offset for the TileWorldPosition to ensure proper rounding
                FVector TileOffset(TilesSize.X / 2, TilesSize.Y / 2, 0);

                // Iterate through the tiles in both X and Y dimensions
                for (int32 i = 0; i < TileCount.X; ++i) {
                    for (int32 j = 0; j < TileCount.Y; ++j) {
                        // Calculate the world position of the tile based on the RelativePosition, TilesSize, and TileOffset
                        FVector TileWorldPosition = RelativePosition + FVector(i * TilesSize.X, j * TilesSize.Y, 0) - TileOffset;
                        // Convert the TileWorldPosition into a FVector2D tile location by dividing by TilesSize and rounding to the nearest integer
                        FVector2D TileLocation(FMath::RoundToInt((TileWorldPosition.X + TilesSize.X / 2) / TilesSize.X), FMath::RoundToInt((TileWorldPosition.Y + TilesSize.Y / 2) / TilesSize.Y));

                        // Add the calculated tile location to the OffTiles array
                        OffTiles.Add(TileLocation);
                    }
                }
            }
        }
    }

    // Return the TArray of FVector2D OffTiles
    return OffTiles;
}






int32 UDungeonGenerationV2Helpers::GetTileCountWithoutOffTiles(TMap<FVector2D, FTileInfo>& TileLayout) {
	int32 TileCount = 0;
	for (const auto& Tile : TileLayout) {
		if (Tile.Value.Type != ETileType::OffTile) {
			TileCount++;
		}
	}
	return TileCount;
}

TArray<FVector2D> UDungeonGenerationV2Helpers::GetTileLayoutWithoutOffTiles(TMap<FVector2D, FTileInfo>& TileLayout, bool bIncludeStartTile) {
	TArray<FVector2D> TileLayoutWithoutOffTiles;
	TileLayout.GenerateKeyArray(TileLayoutWithoutOffTiles);

	TArray<FVector2D> FilteredTileLayout;

	for (const FVector2D& TileLocation : TileLayoutWithoutOffTiles) {
		if (TileLayout[TileLocation].Type == ETileType::OffTile) {
			continue;
		}
		else if (TileLayout[TileLocation].Type == ETileType::StartTile && !bIncludeStartTile) {
			continue;
		}

		FilteredTileLayout.Add(TileLocation);
	}

	return FilteredTileLayout;
}




ECardinalPoints UDungeonGenerationV2Helpers::GetInverseCardinalDirection(ECardinalPoints Direction) {
	switch (Direction) {
	case ECardinalPoints::North:
		return ECardinalPoints::South;
	case ECardinalPoints::East:
		return ECardinalPoints::West;
	case ECardinalPoints::South:
		return ECardinalPoints::North;
	case ECardinalPoints::West:
		return ECardinalPoints::East;
	default:
		return ECardinalPoints::North;
	}
}

ECardinalPoints UDungeonGenerationV2Helpers::FVector2DToCardinalPoint(const FVector2D& Direction) {
	if (Direction == FVector2D(1, 0)) return ECardinalPoints::North;
	if (Direction == FVector2D(0, 1)) return ECardinalPoints::East;
	if (Direction == FVector2D(-1, 0)) return ECardinalPoints::South;
	if (Direction == FVector2D(0, -1)) return ECardinalPoints::West;

	// Fallback, should not happen
	return ECardinalPoints::North;
}

FVector2D UDungeonGenerationV2Helpers::CardinalPointToFVector2D(ECardinalPoints Direction) {
	switch (Direction) {
	case ECardinalPoints::North:
		return FVector2D(1, 0);
	case ECardinalPoints::East:
		return FVector2D(0, 1);
	case ECardinalPoints::South:
		return FVector2D(-1, 0);
	case ECardinalPoints::West:
		return FVector2D(0, -1);
	default:
		return FVector2D(1, 0);
	}
}

EConnectionType UDungeonGenerationV2Helpers::GetConnectionTypeFromDirections(const TArray<bool>& Directions) {
	int32 ConnectionCount = 0;
	for (bool Connection : Directions) {
		if (Connection) {
			ConnectionCount++;
		}
	}

	switch (ConnectionCount) {
		case 1:
			return Single;
		case 2:
			if (Directions[0] == Directions[2] || Directions[1] == Directions[3]) return Line;
			return Corner;
		case 3:
			return Threeway;
		case 4:
			return Fourway;
	}
	return NONE;
}



// DungeonGenerationV2Helpers.cpp
void UDungeonGenerationV2Helpers::UpdateTileDirections(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D PreviousLocation, FVector2D CurrentLocation) {

	if (CurrentLocation == PreviousLocation) return;
	
	FVector2D Direction = CurrentLocation - PreviousLocation;
	ECardinalPoints CardinalDirection = FVector2DToCardinalPoint(Direction);

	// Set the cardinal points of the previous tile
	FTileInfo& PreviousTile = TileLayout.FindChecked(PreviousLocation);
	
	// Set the cardinal points of the current tile
	FTileInfo& CurrentTile = TileLayout.FindChecked(CurrentLocation);
	ECardinalPoints OppositeDirection = GetInverseCardinalDirection(CardinalDirection);

	if (PreviousTile.Type == ETileType::OffTile || CurrentTile.Type == ETileType::OffTile) return;
	PreviousTile.Directions[CardinalDirection] = true;
	CurrentTile.Directions[OppositeDirection] = true;

	
	if (PreviousLocation == FVector2D(1,0) || CurrentLocation == FVector2D(1,0)) {
		UE_LOG(LogTemp, Warning, TEXT("PreviousLocation: %s"), *PreviousLocation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("CurrentLocation: %s"), *CurrentLocation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("TileLayoutCount: %d"), GetTileCountWithoutOffTiles(TileLayout));
	}
	
}

bool UDungeonGenerationV2Helpers::CreatePathToTarget(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D TargetLocation, TEnumAsByte<ECardinalPoints> FinalDirection) {
	// Create a TArray and sort tiles based on their distance to the target location
	TArray<FVector2D> SortedTiles;
	for (const auto& Tile : TileLayout) {
		if (Tile.Value.Type == ETileType::OffTile) continue;
		SortedTiles.Add(Tile.Key);
	}

	//Sort the array based on distance (closest to furthest)
	SortedTiles.Sort([&](const FVector2D& A, const FVector2D& B) {
		return FVector2D::Distance(A, TargetLocation) < FVector2D::Distance(B, TargetLocation);
	});

	// Iterate through the sorted tiles and attempt to find a path
	for (const FVector2D& StartTile : SortedTiles) {
		TArray<FVector2D> Path = AStarPathfinding(TileLayout, StartTile, TargetLocation);

		UE_LOG(LogTemp, Log, TEXT("Path length: %d"), Path.Num());

		// If a path is found, set up the entire path in the tile map and return true
		if (Path.Num() > 0) {
			
			for (int32 i = 0; i < Path.Num() - 1; ++i) {
				FVector2D CurrentLocation = Path[i];
				FVector2D NextLocation = Path[i + 1];

				// Add the path tiles to the TileLayout if they are not already present
				if (!TileLayout.Contains(CurrentLocation)) {
					FTileInfo CurrentTileInfo;
					TileLayout.Add(CurrentLocation, CurrentTileInfo);
				}
				if (!TileLayout.Contains(NextLocation)) {
					FTileInfo NextTileInfo;
					TileLayout.Add(NextLocation, NextTileInfo);
				}

				UpdateTileDirections(TileLayout, CurrentLocation, NextLocation);
			}

			// Set the final direction of the path
			FTileInfo& FinalTile = TileLayout.FindChecked(Path.Last());
			FinalTile.Directions[FinalDirection] = true;
			
			return true;
		}
	}

	// If no valid path is found, return false
	return false;
}




TArray<FVector2D> UDungeonGenerationV2Helpers::AStarPathfinding(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D StartLocation, FVector2D EndLocation) {
    TSet<FVector2D> OpenSet;
    TSet<FVector2D> ClosedSet;
    TMap<FVector2D, TSharedPtr<FDungeonPathNode>> NodeMap;

	
		if ((TileLayout.Contains(StartLocation) && TileLayout[StartLocation].Type == ETileType::OffTile) ||
	    (TileLayout.Contains(EndLocation) && TileLayout[EndLocation].Type == ETileType::OffTile)) {
	    // Return an empty path if either the start or end location is an off-tile
	    return TArray<FVector2D>();
	}

    // Initialize the starting node and add it to the OpenSet
    TSharedPtr<FDungeonPathNode> StartNode = MakeShared<FDungeonPathNode>(StartLocation, FVector2D::Distance(StartLocation, EndLocation), 0, nullptr);
    OpenSet.Add(StartLocation);
    NodeMap.Add(StartLocation, StartNode);

    while (OpenSet.Num() > 0) {
    	UE_LOG(LogTemp, Log, TEXT("OpenSet size: %d"), OpenSet.Num());
        // Find the node with the lowest FScore in the OpenSet
        FVector2D CurrentNodeLocation = GetLowestFScoreNode(OpenSet, NodeMap);

    	UE_LOG(LogTemp, Log, TEXT("CurrentNodeLocation: %s"), *CurrentNodeLocation.ToString());
        // If the current node is the end location, a path has been found
        if (CurrentNodeLocation == EndLocation) {
            TArray<FVector2D> Path = ReconstructPath(NodeMap, CurrentNodeLocation);
            return Path;
        }

        OpenSet.Remove(CurrentNodeLocation);
        ClosedSet.Add(CurrentNodeLocation);

        // Get the neighbors of the current node
        TArray<FVector2D> Neighbors = GetNeighbors(CurrentNodeLocation);

    	for (const FVector2D& NeighborLocation : Neighbors) {
    		// Ignore this neighbor if it's in the ClosedSet or is an OffTile or Invalid Tile
    		if (ClosedSet.Contains(NeighborLocation) || TileLayout.Contains(NeighborLocation)) {
    			continue;
    		}

            float TentativeGScore = NodeMap[CurrentNodeLocation]->GScore + FVector2D::Distance(CurrentNodeLocation, NeighborLocation);

            if (!OpenSet.Contains(NeighborLocation)) {
                OpenSet.Add(NeighborLocation);
            } else if (TentativeGScore >= NodeMap[NeighborLocation]->GScore) {
                continue;
            }

            // Update the neighbor node or create a new one if it doesn't exist
    		if (NodeMap.Contains(NeighborLocation)) {
    			NodeMap[NeighborLocation]->GScore = TentativeGScore;
    			NodeMap[NeighborLocation]->FScore = TentativeGScore + FVector2D::Distance(NeighborLocation, EndLocation);
    			NodeMap[NeighborLocation]->Parent = *NodeMap.Find(CurrentNodeLocation); 
    		} else {
    			TSharedPtr<FDungeonPathNode> NeighborNode = MakeShared<FDungeonPathNode>(NeighborLocation, FVector2D::Distance(NeighborLocation, EndLocation), TentativeGScore, *NodeMap.Find(CurrentNodeLocation));
    			NodeMap.Add(NeighborLocation, NeighborNode);
    		}
        }
    }

    // If no path was found, return an empty path
    return TArray<FVector2D>();
}



FVector2D UDungeonGenerationV2Helpers::GetLowestFScoreNode(TSet<FVector2D>& OpenSet, TMap<FVector2D, TSharedPtr<FDungeonPathNode>>& NodeMap) {
	FVector2D LowestFScoreNodeLocation;
	float MinFScore = MAX_flt;

	for (const FVector2D& Location : OpenSet) {
		if (NodeMap[Location]->FScore < MinFScore) {
			MinFScore = NodeMap[Location]->FScore;
			LowestFScoreNodeLocation = Location;
		}
	}

	return LowestFScoreNodeLocation;
}

TArray<FVector2D> UDungeonGenerationV2Helpers::GetNeighbors(FVector2D CurrentNodeLocation) {
	TArray<FVector2D> Neighbors = {
		CurrentNodeLocation + FVector2D(1, 0),
		CurrentNodeLocation + FVector2D(-1, 0),
		CurrentNodeLocation + FVector2D(0, 1),
		CurrentNodeLocation + FVector2D(0, -1)
		};
	return Neighbors;
}

TArray<FVector2D> UDungeonGenerationV2Helpers::ReconstructPath(TMap<FVector2D, TSharedPtr<FDungeonPathNode>>& NodeMap, FVector2D CurrentNodeLocation) {
	TArray<FVector2D> Path;

	if (!NodeMap.Contains(CurrentNodeLocation)) {
		UE_LOG(LogTemp, Error, TEXT("ReconstructPath: NodeMap does not contain CurrentNodeLocation."));
		return Path;
	}

	TSharedPtr<FDungeonPathNode> CurrentNode = NodeMap[CurrentNodeLocation];
	TSharedPtr<FDungeonPathNode> PreviousNode = nullptr;

	while (CurrentNode.IsValid()) {
		if (PreviousNode == CurrentNode) {
			UE_LOG(LogTemp, Error, TEXT("ReconstructPath: Encountered an infinite loop."));
			break;
		}

		Path.Add(CurrentNode->Location);
		PreviousNode = CurrentNode;

		if (CurrentNode->Parent.IsValid()) {
			if (NodeMap.Contains(CurrentNode->Parent->Location)) {
				CurrentNode = CurrentNode->Parent;
			} else {
				UE_LOG(LogTemp, Error, TEXT("ReconstructPath: NodeMap does not contain the parent node's location."));
				break;
			}
		} else {
			UE_LOG(LogTemp, Error, TEXT("ReconstructPath: Encountered an invalid parent node."));
			break;
		}
	}

	// Reverse the path to start from the beginning
	Algo::Reverse(Path);

	return Path;
}









