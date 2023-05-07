// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DungeonGeneration/Prefabinator.h"
#include "AI/EnemyData.h"
#include "AI/EnemySpawnPoint.h"
#include "AI/AIGroupManager.h"
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
	
	// Generative Tile Placement and Backtracking Helpers */
	// This section doesn't contain functions regarding A* Pathfinding, those are further below */
	
	/** Finds the next random location based on the current location and the provided constraints.
	 * The function uses the Fisher-Yates shuffle algorithm to randomize the order of the valid locations.
	 * It also takes into account a preference for locations further away from other tiles to make the level feel more open and dynamic.
	 *
	 * @param TileLayout The current layout of the dungeon
	 * @param CurrentLocation The current location of the generator
	 * @param Seed The stream used for generating random values
	 * @param MaxCorridorsInRow The maximum number of consecutive corridors allowed in a row
	 * @param TargetLocation The target location to avoid, typicaly used to prevent the generator from getting too close to a target in hybrid mode
	 * @param MinDistanceFromTarget The minimum distance that should be maintained from the target location
	 * @param bUseFisherYates Use FisherYates shuffle algorithm to randomize the order of the valid locations. If false, uses location weighting.
	 *
	 * @return A FVector2D representing the next location chosen by the generator, based on the provided constraints
	 */
	UFUNCTION()
	static FVector2D FindNextLocationRandom(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D& CurrentLocation, FRandomStream& Seed, 
	int32 MaxCorridorsInRow = 2, FVector2D TargetLocation = FVector2D(0,0), float MinDistanceFromTarget = 0, bool bUseFisherYates = true);


	/** Finds the next location in a specified direction from the current location.
	 * Mostly used for mandetory tiles (i.e. the start and end tiles)
	 * 
	 * @param TileLayout The current layout of the dungeon
	 * @param CurrentLocation The current location in the dungeon
	 * @param Direction A cardinal direction to move towards
	 * 
	 * @return The next location in the specified direction
	 */
	UFUNCTION()
	static FVector2D FindNextLocationSpecified(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D& CurrentLocation, ECardinalPoints Direction);

	/** Backtracks through the tile layout and returns the next random location.
	 * This function is used in conjunction with FindNextLocationRandom for implementing recursive backtracking.
	 * 
	 * @param TileLayout The current layout of the dungeon
	 * @param CurrentLocation The current location in the dungeon
	 * @param Seed The random stream used for generating random numbers
	 * @param MaxCorridorsInRow The maximum number of corridors in a row
	 * @param TargetLocation The target location in the dungeon
	 * @param MinDistanceFromTarget The minimum distance from the target location
	 * @param bSkipFirst Whether or not to skip the first location found by backtracking
	 * 
	 * @return The next random location found by backtracking
	 */
	UFUNCTION()
	static FVector2D BacktrackerRandom(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D& CurrentLocation, FRandomStream& Seed, int32 MaxCorridorsInRow = 2,
	    FVector2D TargetLocation = FVector2D(0,0), float MinDistanceFromTarget = 0, bool bSkipFirst = false);

	/** Branches out random paths from the existing dungeon layout to create a more open and dynamic level.
	 * The function picks random points on the generated layout and tries to branch out a random number of times.
	 * Takes in some parameters used for calling FindNextLocationRandom.
	 * 
	 * @param TileLayout The current layout of the dungeon
	 * @param CurrentLocation The current location in the dungeon
	 * @param Seed The random stream used for generating random numbers
	 * @param MaxCorridorsInRow The maximum number of corridors in a row
	 * @param BranchLength The length of the branches to be added to the dungeon
	 */
	UFUNCTION()
	static bool BranchOutRandomPaths(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D& CurrentLocation, FRandomStream& Seed, int32 MaxCorridorsInRow,
	    int32 BranchLength);



	
    // Tile Matching and Spawning Helpers */
	
    /* Finds UClass instances of tiles that match the given TileInfo from the SpawnableTiles array */
    UFUNCTION()
    static TArray<UClass*> FindMatchingTileClasses(const FTileInfo& TileInfo, TArray<UClass*>& SpawnableTiles);

    /* Checks if the TileInfo and TileBase have matching connection types */
    UFUNCTION()
    static bool DoTileInfoAndTileBaseMatch(const FTileInfo& TileInfo, const ATileBase* TileBase);

    /* Gets the rotation required for the ATileBase to match the given TileInfo */
    static FRotator GetTileRotation(const FTileInfo& TileInfo, const ATileBase* TileBase);

    /* Spawns a tile of a given UClass at the specified location and direction */
    UFUNCTION()
    static ATileBase* SpawnTile(UClass* TileToSpawn, FVector2D Location, ECardinalPoints Direction, UWorld* World);

	
	// World Getters */
	
	/* Get all prefabinators in a tile streaming level */
	UFUNCTION()
	static bool GetPrefabinatorsInLevel(UWorld* InWorld, TArray<class APrefabinator*>& OutPrefabinators);

	/* Get all enemyspawnpoints in a tile streaming level */
	UFUNCTION()
	static bool GetEnemySpawnPointsInLevel(UWorld* InWorld, TArray<class AEnemySpawnPoint*>& OutEnemySpawnPoints);

	/* Get all groupmanagers in a tile streaming level */
	UFUNCTION()
	static bool GetGroupManagersInLevel(UWorld* InWorld, TArray<class AAIGroupManager*>& OutGroupManagers);

	/* Get all Spawnpoints in the world */
	UFUNCTION()
	static bool GetSpawnPointsInWorld(UWorld* InWorld, TArray<class APSpawnPoint*>& OutSpawnPoints);

	/* Get all levelgeneratorsV2 in the world */
	UFUNCTION()
	static bool GetLevelGeneratorsInWorld(UWorld* InWorld, TArray<class ALevelGeneratorV2*>& OutLevelGenerators);

	/* Get all lootcrates in the world */
	UFUNCTION()
	static bool GetLootCratesInWorld(UWorld* InWorld, TArray<class ALootCrate*>& OutLootCrates);
	
	
    // Tile Layout Helpers */
	
    /* Updates the directions of the tiles in the TileLayout based on the previous and current locations */
    UFUNCTION()
    static void UpdateTileDirections(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D PreviousLocation, FVector2D CurrentLocation);

    /* Checks if the next location in the TileLayout is a corridor and if it reaches the maximum allowed corridors in a row	*/
    UFUNCTION()
    static bool IsNextLocationCorridor(const TMap<FVector2D, FTileInfo>& TileLayout, FVector2D NextLocation, int32 MaxCorridorsInRow);

	//
	UFUNCTION()
	static float CalculateNeighboringTilesDistanceSum(const TMap<FVector2D, FTileInfo>& TileLayout, const FVector2D& Location);

	/* Checks the offtilearea actors in the world, converting them to local-space based on the scale, then adds these locations to the TileLayout */
	UFUNCTION()
	static TArray<FVector2D> UpdateOffTilesFromWorldComponents(TArray<AActor*> OffTileAreaActors, FVector TilesSize, FVector LevelGeneratorPosition);

	/* Count the number of tiles in the TileLayout that are not off tiles */
	UFUNCTION()
	static int32 GetTileCountWithoutOffTiles(TMap<FVector2D, FTileInfo>& TileLayout);

	/* Returns an array of the locations of the tiles in the TileLayout without off tiles */
	UFUNCTION()
	static TArray<FVector2D> GetTileLayoutWithoutOffTiles(TMap<FVector2D, FTileInfo>& TileLayout, bool bIncludeStartTile = true);

	
	/* A* Pathfinding Algorithm Helpers */

	/** Creates a path to the target location using the A* pathfinding algorithm, updating the tile layout with the path.
	 *
	 * @param TileLayout The current layout of the dungeon
	 * @param TargetLocation The target location in the dungeon
	 * @param FinalDirection The final direction of the path
	 * 
	 * @return True if a path was successfully created, false otherwise
	 */
	UFUNCTION()
	static bool CreatePathToTarget(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D TargetLocation, TEnumAsByte<ECardinalPoints> FinalDirection = North);

	/** Executes the A* pathfinding algorithm and returns an array of FVector2D representing the path.
	 *
	 * @param TileLayout The current layout of the dungeon
	 * @param StartLocation The starting location in the dungeon
	 * @param EndLocation The target location in the dungeon
	 * 
	 * @return An array of FVector2D representing the calculated path
	 */
	UFUNCTION()
	static TArray<FVector2D> AStarPathfinding(TMap<FVector2D, FTileInfo>& TileLayout, FVector2D StartLocation, FVector2D EndLocation);

	/** Returns the location of the node with the lowest FScore in the OpenSet.
	 *
	 * @param OpenSet The set of nodes to be evaluated
	 * @param NodeMap The map containing the node data
	 * 
	 * @return The location of the node with the lowest FScore
	 */
	static FVector2D GetLowestFScoreNode(TSet<FVector2D>& OpenSet, TMap<FVector2D, TSharedPtr<FDungeonPathNode>>& NodeMap);

	/** Returns an array of FVector2D representing the neighboring nodes of the given node location.
	 *
	 * @param CurrentNodeLocation The location of the current node
	 * 
	 * @return An array of FVector2D representing the neighboring nodes
	 */
	UFUNCTION()
	static TArray<FVector2D> GetNeighbors(FVector2D CurrentNodeLocation);

	/** Reconstructs the path from the A* algorithm based on the NodeMap and the given node location.
	 *
	 * @param NodeMap The map containing the node data
	 * @param CurrentNodeLocation The location of the current node
	 * 
	 * @return An array of FVector2D representing the reconstructed path
	 */
	static TArray<FVector2D> ReconstructPath(TMap<FVector2D, TSharedPtr<FDungeonPathNode>>& NodeMap, FVector2D CurrentNodeLocation);



	
	// Cardinal Point Helpers */
	
	/* Returns the inverse of the given cardinal direction */
	UFUNCTION()
	static ECardinalPoints GetInverseCardinalDirection(ECardinalPoints Direction);

	/* Converts a cardinal direction into a FVector2D */
	UFUNCTION()
	static FVector2D CardinalPointToFVector2D(ECardinalPoints Direction);

	/* Converts a FVector2D into a cardinal direction */
	UFUNCTION()
	static ECardinalPoints FVector2DToCardinalPoint(const FVector2D& Direction);

	/* Determines the connection type based on an array of booleans representing directions */
	UFUNCTION()
	static EConnectionType GetConnectionTypeFromDirections(const TArray<bool>& Directions);

	/* Cardinal Directions array */
	static inline const FVector2D CardinalDirections[] = {
		FVector2D(1, 0),  // North
		FVector2D(0, 1),  // East
		FVector2D(-1, 0), // South
		FVector2D(0, -1)  // West
	};


	
};
