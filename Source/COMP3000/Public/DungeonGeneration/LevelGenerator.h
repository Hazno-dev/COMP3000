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
	UClass* StartTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<UClass*> GenericTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	UClass* EndTile;
	
	//Defining data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FVector TilesSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int ValueAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TEnumAsByte<ECardinalPoints> StartDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TEnumAsByte<ECardinalPoints> OffDirection;
	
	//Generation Functions
	UFUNCTION()
	void TileLayoutGeneration();

	UFUNCTION()
	void SpawnTileLayout();
	
	UFUNCTION()
	void SpawnTile(UClass* TileClass, const FVector& position);

	UFUNCTION()
	FVector GetTileWorldspace(FVector2D InVec);

	UFUNCTION()
	FVector2D FindNextRandomLocation(FVector2D InVec, TEnumAsByte<ECardinalPoints> WantedDirection);

	UFUNCTION()
	void TileConnector(FVector2D FirstTileVec, FVector2D SecondTileVec, TEnumAsByte<ECardinalPoints> FirstCardinal);

	UFUNCTION()
	void SetConnection(FVector2D InTileVec, TEnumAsByte<ECardinalPoints> ToCardinal, bool enable);

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FVector2D PreviousLocation;
	FVector2D CurrentLocation;
	TMap<ECardinalPoints, FVector2D> SeekingPoints;
	TEnumAsByte<ECardinalPoints> TempDirect;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
