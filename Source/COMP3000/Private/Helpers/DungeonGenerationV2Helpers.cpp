// Callum Stables, 2023


#include "Helpers/DungeonGenerationV2Helpers.h"

FVector2D UDungeonGenerationV2Helpers::FindNextLocationRandom(TMap<FVector2D, FTileInfo>& TileLayout,
	FVector2D CurrentLocation, FRandomStream& Seed, int32 MaxCorridorsInRow, FVector2D TargetLocation, float MinDistanceFromTarget) {

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

	// Shuffle the valid locations and directions using Fisher-Yates shuffle
	for (int32 i = ValidLocations.Num() - 1; i > 0; --i) {
		int32 j = Seed.RandRange(0, i);
		ValidLocations.Swap(i, j);
		ValidDirections.Swap(i, j);
	}

	// Iterate over shuffled valid locations
	for (int32 i = 0; i < ValidLocations.Num(); ++i) {
		FVector2D SelectedLocation = ValidLocations[i];

		// Check if the selected location would form a corridor
		if (!IsNextLocationCorridor(TileLayout, SelectedLocation, MaxCorridorsInRow)) {
			return SelectedLocation;
		}
	}

	// If no valid location is found, use the Backtrack helper function
	return BacktrackerRandom(TileLayout, CurrentLocation, Seed);
}

FVector2D UDungeonGenerationV2Helpers::FindNextLocationTargeted(TMap<FVector2D, FTileInfo>& TileLayout,
	FVector2D CurrentLocation, FVector2D TargetLocation, FRandomStream& Seed) {
	return FVector2D::ZeroVector;
}

FVector2D UDungeonGenerationV2Helpers::FindNextLocationSpecified(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D CurrentLocation, ECardinalPoints Direction) {
	FVector2D NextLocation = CurrentLocation + CardinalPointToFVector2D(Direction);

	// Check if the next location is not occupied
	if (!TileLayout.Contains(NextLocation)) {
		return NextLocation;
	}

	// If the next location is already occupied, return the current location
	return CurrentLocation;
}


FVector2D UDungeonGenerationV2Helpers::BacktrackerRandom(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D CurrentLocation, FRandomStream& Seed) {
	TArray<FVector2D> TileLocations;
	TileLayout.GenerateKeyArray(TileLocations);

	int counter = TileLocations.Num() - 1;
	FVector2D returnVec = CurrentLocation;
	
	for (counter; counter > 1; counter--) {
		returnVec = TileLocations[counter];
		FVector2D NextLocation = FindNextLocationRandom(TileLayout, returnVec, Seed);

		// If the next location is not the current location, we have found a valid location.
		if (returnVec != NextLocation) {
			return NextLocation;
		}
	}

	return CurrentLocation;
}

TArray<UClass*> UDungeonGenerationV2Helpers::FindMatchingTileClasses(const FTileInfo& TileInfo, TArray<UClass*>& SpawnableTiles) {
	TArray<UClass*> MatchingTileClasses;
	for (UClass* TileClass : SpawnableTiles) {
		ATileBase* TileBase = Cast<ATileBase>(TileClass->GetDefaultObject());

		if (!TileBase) {
			UE_LOG(LogTemp, Warning, TEXT("Tile class does not inherit from ATileBase. Skipping."));
			continue;
		}

		if (DoTileInfoAndTileBaseMatch(TileInfo, TileBase)) {
			MatchingTileClasses.Add(TileClass);
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Tile info and tile base do not match for class: %s"), *TileClass->GetName());
		}
	}
	return MatchingTileClasses;
}


bool UDungeonGenerationV2Helpers::DoTileInfoAndTileBaseMatch(const FTileInfo& TileInfo, const ATileBase* TileBase) {
	// Check if the TileType matches
	if (TileInfo.Type != TileBase->TileType) {
		UE_LOG(LogTemp, Warning, TEXT("Tile type does not match"));
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

		switch (TileBase->ConnectionType) {
		case Threeway:
			// Handle Threeway rotation logic
			if (CurrentTileMapDirections[0] && CurrentTileMapDirections[1] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[2]) Yaw = 0.0f;
			else if (CurrentTileMapDirections[0] && CurrentTileMapDirections[1] && CurrentTileMapDirections[2] && !CurrentTileMapDirections[3]) Yaw = 90.0f;
			else if (CurrentTileMapDirections[1] && CurrentTileMapDirections[2] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[0]) Yaw = 180.0f;
			else if (CurrentTileMapDirections[0] && CurrentTileMapDirections[2] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[1]) Yaw = 270.0f;
			break;
		case Fourway:
			// Handle Fourway rotation logic (no rotation required)
			break;
		case Line:
			// Handle Line rotation logic
			if (CurrentTileMapDirections[0] && CurrentTileMapDirections[2] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[3]) Yaw = 0.0f;
			else if (CurrentTileMapDirections[1] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[2]) Yaw = 90.0f;
			break;
		case Single:
			// Handle Single rotation logic
			if (CurrentTileMapDirections[0] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[2] && !CurrentTileMapDirections[3]) Yaw = 0.0f;
			else if (CurrentTileMapDirections[1] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[2] && !CurrentTileMapDirections[3]) Yaw = 90.0f;
			else if (CurrentTileMapDirections[2] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[3]) Yaw = 180.0f;
			else if (CurrentTileMapDirections[3] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[2]) Yaw = 270.0f;
			break;
		case Corner:
			// Fallback to cardinal directions (also handles Corner case)
			if (CurrentTileMapDirections[0] && CurrentTileMapDirections[1] && !CurrentTileMapDirections[2] && !CurrentTileMapDirections[3]) Yaw = 0;
			if (CurrentTileMapDirections[1] && CurrentTileMapDirections[2] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[3]) Yaw = 90;
			if (CurrentTileMapDirections[2] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[0]) Yaw = 180;
			if (CurrentTileMapDirections[3] && CurrentTileMapDirections[0] && !CurrentTileMapDirections[2] && !CurrentTileMapDirections[1]) Yaw = 270;
		}
	}
	return FRotator(0.0f, Yaw, 0.0f);
}

ATileBase* UDungeonGenerationV2Helpers::SpawnTile(UClass* TileToSpawn, FVector2D Location, ECardinalPoints Direction,
                                                  UWorld* World) {
	return nullptr;
}


bool UDungeonGenerationV2Helpers::IsNextLocationCorridor(const TMap<FVector2D, FTileInfo>& TileLayout,
														 FVector2D NextLocation, int32 MaxCorridorsInRow) {

	int32 CorridorCount = 0;

	// Iterate through all directions
	for (const FVector2D& Direction : CardinalDirections) {
		FVector2D AdjacentLocation = NextLocation + Direction;
		FTileInfo AdjacentTileInfo = TileLayout.FindRef(AdjacentLocation);

		// If the tile is a starting tile, ignore it
		if (AdjacentTileInfo.Type == ETileType::StartTile) continue;

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
	FVector2D Direction = CurrentLocation - PreviousLocation;
	ECardinalPoints CardinalDirection = FVector2DToCardinalPoint(Direction);

	// Set the cardinal points of the previous tile
	FTileInfo& PreviousTile = TileLayout.FindChecked(PreviousLocation);
	PreviousTile.Directions[CardinalDirection] = true;

	// Set the cardinal points of the current tile
	FTileInfo& CurrentTile = TileLayout.FindChecked(CurrentLocation);
	ECardinalPoints OppositeDirection = GetInverseCardinalDirection(CardinalDirection);
	CurrentTile.Directions[OppositeDirection] = true;
}



FVector2D UDungeonGenerationV2Helpers::FindClosestLocationToTarget(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D TargetLocation) {
	FVector2D ClosestLocation;
	float MinDistance = FLT_MAX;

	for (const auto& TileEntry : TileLayout) {
		float Distance = FVector2D::Distance(TileEntry.Key, TargetLocation);
		if (Distance < MinDistance) {
			MinDistance = Distance;
			ClosestLocation = TileEntry.Key;
		}
	}

	return ClosestLocation;
}


