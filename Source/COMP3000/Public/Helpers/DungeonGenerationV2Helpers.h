// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DungeonGeneration/GenerationEnums.h"
#include "DungeonGeneration/TileBase.h"
#include "DungeonGenerationV2Helpers.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UDungeonGenerationV2Helpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static FVector2D FindNextLocationRandom(TMap<FVector2D, FTileInfo>& TileLayout,
		FVector2D CurrentLocation, FRandomStream& Seed, int32 MaxCorridorsInRow = 2, FVector2D TargetLocation = FVector2D(0,0), float MinDistanceFromTarget = 0);

	UFUNCTION()
	static FVector2D FindNextLocationTargeted(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D CurrentLocation, FVector2D TargetLocation, FRandomStream& Seed);

	UFUNCTION()
	static FVector2D FindNextLocationSpecified(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D CurrentLocation, ECardinalPoints Direction);

	/** Uses the FindNextLocationRandom function to backtrack through the tilelayout based on the invector */
	UFUNCTION()
	static FVector2D BacktrackerRandom(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D CurrentLocation, FRandomStream& Seed);

	UFUNCTION()
	static TArray<UClass*> FindMatchingTileClasses(const FTileInfo& TileInfo, TArray<UClass*>& SpawnableTiles);

	UFUNCTION()
	static bool DoTileInfoAndTileBaseMatch(const FTileInfo& TileInfo, const ATileBase* TileBase);

	static FRotator GetTileRotation(const FTileInfo& TileInfo, const ATileBase* TileBase);
	
	UFUNCTION()
	static ATileBase* SpawnTile(UClass* TileToSpawn, FVector2D Location, ECardinalPoints Direction, UWorld* World);
	
	UFUNCTION()
	static ECardinalPoints GetInverseCardinalDirection(ECardinalPoints Direction);

	UFUNCTION()
	static FVector2D CardinalPointToFVector2D(ECardinalPoints Direction);

	UFUNCTION()
	static ECardinalPoints FVector2DToCardinalPoint(const FVector2D& Direction);

	UFUNCTION()
	static EConnectionType GetConnectionTypeFromDirections(const TArray<bool>& Directions);

	UFUNCTION()
	static void UpdateTileDirections(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D PreviousLocation, FVector2D CurrentLocation);
	
	UFUNCTION()
	static bool IsNextLocationCorridor(const TMap<FVector2D, FTileInfo>& TileLayout, FVector2D NextLocation, int32 MaxCorridorsInRow);

	UFUNCTION()
	static FVector2D FindClosestLocationToTarget(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D TargetLocation);
	
	static inline const FVector2D CardinalDirections[] = {
		FVector2D(1, 0),  // North
		FVector2D(0, 1),  // East
		FVector2D(-1, 0), // South
		FVector2D(0, -1)  // West
	};


	
};
