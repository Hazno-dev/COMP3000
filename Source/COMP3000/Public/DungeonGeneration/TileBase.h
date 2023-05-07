// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenerationEnums.h"
#include "AI/EnemyData.h"
#include "AI/EnemySpawnPoint.h"
#include "GameFramework/Actor.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "Engine/LevelStreamingDynamic.h"
#include "TileBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelCustomLoadedDelegate, ATileBase*, Tile);
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
	int32 MinMinionsToSpawn;

	UPROPERTY()
	int32 MinEliteToSpawn;

	UPROPERTY()
	int32 MaxMinionsToSpawn;

	UPROPERTY()
	int32 MaxEliteToSpawn;
	
	UPROPERTY()
	ULevelStreamingDynamic* LevelInstance;

	UPROPERTY(EditAnywhere, Category = Level, meta = (NoCreate, DisplayName="Level"))
	TSoftObjectPtr<UWorld> WorldAsset;

	/* Is the tile able to rotate along every axis */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	bool bMultiDirectional;

	UPROPERTY()
	bool bIsLevelLoaded;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile Data")
	TArray<AEnemySpawnPoint*> TileEnemySpawnPoints;

	UPROPERTY(BlueprintAssignable, Category = "Tile Events")
	FOnLevelCustomLoadedDelegate OnLevelLoadedCustom;

	UFUNCTION()
	void TileLoaded();

	UFUNCTION()
	void SetupDelegate();

	UFUNCTION()
	int32 SpawnInitialEnemies(TArray<FEnemyData> EnemyData, FRandomStream SeedStream);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
