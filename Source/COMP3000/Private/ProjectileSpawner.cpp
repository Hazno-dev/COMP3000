// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSpawner.h"

// Sets default values for this component's properties
UProjectileSpawner::UProjectileSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


void UProjectileSpawner::SpawnProjectile(FVector Location, FRotator Rotation) {
	if (ProjectileClass == nullptr) return;
	
	AProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, Location, Rotation, FActorSpawnParameters());

	if (!IsValid(SpawnedProjectile)) return;
	
	SpawnedProjectile->ProjectileMovement->Velocity *= ProjectileSpeed;
	SpawnedProjectile->Damage = Damage;
	SpawnedProjectile->SetOwner(this->GetOwner());
}

// Called when the game starts
void UProjectileSpawner::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UProjectileSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

