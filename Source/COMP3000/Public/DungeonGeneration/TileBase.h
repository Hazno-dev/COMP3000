// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenerationEnums.h"
#include "GameFramework/Actor.h"
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
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
