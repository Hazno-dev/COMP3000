// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIGroupManager.h"

// Sets default values
AAIGroupManager::AAIGroupManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

}

// Called when the game starts or when spawned
void AAIGroupManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAIGroupManager::OnConstruction(const FTransform& Transform) {
	
	SphereComponent->SetSphereRadius(SphereRadius);
}

// Called every frame
void AAIGroupManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

