// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "DungeonGeneration/GenerationEnums.h"
#include "DungeonGeneration/TileBase.h"
#include "GameFramework/Actor.h"
#include "LevelGeneratorV2.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTileBaseSpawned, ALevelGeneratorV2*, InGenerator, TArray<ATileBase*>, InSpawnedTiles);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAllLevelsGenerated);
UCLASS()
class COMP3000_API ALevelGeneratorV2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGeneratorV2();
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	//Generated Layout
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	TMap<FVector2D, FTileInfo> TileLayout;

	//Defining data
	/** Generation Mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	EGeneratorMode Mode;

	/** Tile references */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<UClass*> SpawnableTiles;
	
	/** Real-World Size Of Tile BPs */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FVector TilesSize;
	
	/** How Many Generics Should Be Created (Length Of Dungeon) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (EditCondition = "Mode == EGeneratorMode::Hybrid || Mode == EGeneratorMode::FullyRandom"))
	int32 Length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (EditCondition = "Mode == EGeneratorMode::Hybrid || Mode == EGeneratorMode::FullyRandom", ClampMax = 50))
	int32 StepBackChance;

	/** Is Seed Random */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsSeedRandom;
	
	/** The Seed of the Dungeon - Use to Generate the Same Dungeon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation", meta = (EditCondition = "!bIsSeedRandom"))
	int32 Seed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (EditCondition = "Mode == EGeneratorMode::Hybrid || Mode == EGeneratorMode::FullyRandom", ClampMin = 2))
	int32 MaxCorridorsInRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (EditCondition = "Mode == EGeneratorMode::Hybrid || Mode == EGeneratorMode::Fixed"))
	FVector2D TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (EditCondition = "Mode == EGeneratorMode::Hybrid"))
	int32 MinDistanceFromTarget;

	/** Direction To Start */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TEnumAsByte<ECardinalPoints> StartDirection;

	/** Direction To End */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (EditCondition = "Mode == EGeneratorMode::Hybrid || Mode == EGeneratorMode::Fixed"))
	TEnumAsByte<ECardinalPoints> EndDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bOutBranching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (EditCondition = "bOutBranching"))
	int32 OutBranchingCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (EditCondition = "bOutBranching", ClampMin = 1))
	int32 OutBranchingMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (EditCondition = "bOutBranching", ClampMin = 1))
	int32 OutBranchingMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bHasKeyTile;

	/* Data Table of enemies */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* EnemyDataTable;
	
	UFUNCTION()
	void ResetAndRegenerate();

	UPROPERTY()
	TArray<ATileBase*> SpawnedTiles;

	UFUNCTION()
	TArray<FEnemyData> GetEnemyData() const { return EnemyData; }

	UFUNCTION()
	FRandomStream GetSeedStream() const { return SeedStream; }

	FTileBaseSpawned OnTileBaseSpawned;
	FAllLevelsGenerated OnAllLevelsGenerated;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void InitializeDungeon();

	UFUNCTION()
	void StartingTile();
	
	UFUNCTION()
	void GenerateDungeonRandom(int32 Amount);

	UFUNCTION()
	void GenerateDungeonHybrid(int32 Amount);

	UFUNCTION()
	void GenerateDungeonFixed();

	UFUNCTION()
	bool GenerateBranches();

	UFUNCTION()
	bool SpawnKeyTile();

	UFUNCTION()
	void SpawnDungeonTiles();

	UFUNCTION()
	bool SafetyChecks();

	UFUNCTION()
	void UpdateOffTilesFromWorldComponents();

	UFUNCTION()
	void TileLoaded(ATileBase* Tile);

	//UFUNCTION()
	//void AllTilesLoaded();
	
	FRandomStream SeedStream;
	FVector2D CurrentLocation;
	TArray<FVector2D> OffTiles;
	TArray<FEnemyData> EnemyData;

	//Tile loaded count
	int32 MaxLoadedTilesCount;
	int32 LoadedTilesCount;

	int32 SpawnedEnemyCount;
	
	/** Vectors In Here Won't Be Considered For Generation */
	
	

};
