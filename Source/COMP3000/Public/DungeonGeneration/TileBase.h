// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenerationEnums.h"
#include "GameFramework/Actor.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "Engine/LevelStreamingDynamic.h"
#include "TileBase.generated.h"

UCLASS()
class COMP3000_API ATileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	TEnumAsByte<ETileDirection> TileDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	TMap<TEnumAsByte<ECardinalPoints>, bool> TileConnections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	TEnumAsByte<ETileType> TileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	TEnumAsByte<EConnectionType> ConnectionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	FVector2D TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	int32 MaxInstances;
	
	UPROPERTY()
	ULevelStreamingDynamic* LevelInstance;

	UPROPERTY(EditAnywhere, Category = Level, meta = (NoCreate, DisplayName="Level"))
	TSoftObjectPtr<UWorld> WorldAsset;

	/* Is the tile able to rotate along every axis */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	bool bMultiDirectional;

	UPROPERTY()
	bool bIsLevelLoaded;

	UFUNCTION()
	void SetSeedStream(FRandomStream& InSeedStream) { SeedStream = InSeedStream; }

	UFUNCTION()
	void SpawnPrefabinatorsRandomly();

	UFUNCTION()
	void SetupDelegate();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FRandomStream SeedStream;





};
