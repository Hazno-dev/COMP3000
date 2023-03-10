// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenerationEnums.h"
#include "TileBase.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

UCLASS()
class COMP3000_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

	//Generated Layout
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	TMap<FVector2D, FTileInfo> TileLayout;

	//Tile references
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<UClass*> StartTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<UClass*> GenericTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<UClass*> EndTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<UClass*> AdditionalTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<UClass*> SpecialTiles;
	
	//Defining data
	/** Real-World Size Of Tile BPs */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FVector TilesSize;

	/** How Many Generics Should Be Created (Length Of Dungeon) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int ValueAmount;

	/** How Many Links Should Be Made */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int ExtraLinksAmount;

	/** How Many W/E N/S Can Spawn In A Row */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int MaxInRowAmount;

	/** Amount Of Extra Tiles To Add */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int AdditionalTilesAmount;

	/** Direction To Start */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TEnumAsByte<ECardinalPoints> StartDirection;

	/** Vectors In Here Won't Be Considered For Generation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FVector2D> OffTiles;

	/** Chance Of Stepping Back Each Time A Tile Is Generated */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta=(ClampMin = 0.0f, ClampMax = 50.0f))
	float StepBackChance;

	/** Minimum Distance Start-End Needs To Be (Straight line from A-B) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int MinStartEndDistance;

	/** Spawn End Tile Furthest From Start? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool SpawnEndFurthest;

	/* Show Debug Tiles? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bShowDebugTiles;
	
	//Generation Functions
	UFUNCTION()
	void GenerateLevel();

	UFUNCTION()
	void ResetGeneratedLevel();
	
	UFUNCTION()
	void TileLayoutGeneration();

	UFUNCTION()
	void SpawnTileLayout();
	
	UFUNCTION()
	void SpawnTile(UClass* TileClass, const FVector& position, int Rotation);

	UFUNCTION()
	FVector GetTileWorldspace(FVector2D InVec);

	UFUNCTION()
	FVector2D FindNextRandomLocation(FVector2D InVec, TEnumAsByte<ECardinalPoints> WantedDirection);

	UFUNCTION()
	FVector2D Backtracker(FVector2D InVec, TEnumAsByte<ECardinalPoints> WantedDirection);

	UFUNCTION()
	void GenerateAdditionalTiles();
	
	UFUNCTION()
	void GenerateSpecialTiles();

	UFUNCTION()
	void TileConnector(FVector2D FirstTileVec, FVector2D SecondTileVec, TEnumAsByte<ECardinalPoints> FirstCardinal);

	UFUNCTION()
	void SetConnection(FVector2D InTileVec, TEnumAsByte<ECardinalPoints> ToCardinal, bool enable);

	UFUNCTION()
	void GenerateNewLinks();

	UFUNCTION()
	bool AreTilesConnected(FVector2D InTile, TEnumAsByte<ECardinalPoints> Direction);

	UFUNCTION()
	TEnumAsByte<ECardinalPoints> DirectionInverter(TEnumAsByte<ECardinalPoints> Direction);

	UFUNCTION()
	void SetOffTiles();

	UFUNCTION()
	bool IsThisCorridor(FVector2D InVec, TEnumAsByte<ECardinalPoints> Direction);

	UFUNCTION()
	FVector2D StepBackOne(FVector2D InVec);

	UFUNCTION()
	int DistanceBetweenVec(FVector2D Vec1, FVector2D Vec2);

	UFUNCTION()
	bool IsStartEndTooClose();

	UFUNCTION()
	FVector2D FurthestFromStart(FVector2D InVec, TEnumAsByte<ECardinalPoints> Direction);
	
	
	UClass* LinkAssetTiles(TTuple<UE::Math::TVector2<double>, FTileInfo> InTile, TArray<UClass*> InList);

	bool LinkAssetTile(TTuple<UE::Math::TVector2<double>, FTileInfo> InTile, UClass* InClass);

	TEnumAsByte<EConnectionType> GetConnectionType(TArray<bool> Direction);

	int RotationAmount(FTileInfo InTile);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FVector2D PreviousLocation;
	FVector2D CurrentLocation;
	TMap<ECardinalPoints, FVector2D> SeekingPoints;
	TEnumAsByte<ECardinalPoints> TempDirect;
	TArray<ATileBase*> SpawnedTiles;
	APlayerController* PlayerController;
	int CorridorsSpawned;
	TArray<UClass*> SpecialTilesCloned;
	int CurrentResetAttempts;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
