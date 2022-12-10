// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGeneration/TileBase.h"

// Sets default values
ATileBase::ATileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TileConnections.Add(ECardinalPoints::North, false);
	TileConnections.Add(ECardinalPoints::East, false);
	TileConnections.Add(ECardinalPoints::South, false);
	TileConnections.Add(ECardinalPoints::West, false);
	TileConnections.Add(ECardinalPoints::Random, false);
}

// Called when the game starts or when spawned
void ATileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

