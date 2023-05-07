// Callum Stables, 2023


#include "Heroes/Abilities/AB_VoidSlam.h"

#include "Kismet/GameplayStatics.h"


void AAB_VoidSlam::BeginAbility() {
	Super::BeginAbility();

	if (!IsValid(SplashDamageContainer)) return;
	
	ASplashDamageContainer* SpawnedSplash = WorldRef->SpawnActor<ASplashDamageContainer>(SplashDamageContainer, CastLocation,
		FRotator::ZeroRotator, FActorSpawnParameters());
	SpawnedSplash->SetOwner(GetOwner());

	float SpawnInterval = FMath::RandRange(0.1f, 0.2f);
	WorldRef->GetTimerManager().SetTimer(ArrowSpawnTimerHandle, this, &AAB_VoidSlam::SpawnArrows, SpawnInterval, true);

	if (StartSound && StartAttenuation) UGameplayStatics::PlaySoundAtLocation(WorldRef, StartSound, CastLocation, FRotator::ZeroRotator, 0.5f, 1.0f, 0.0f, StartAttenuation);
	else if (StartSound) UGameplayStatics::PlaySoundAtLocation(WorldRef, StartSound, CastLocation, FRotator::ZeroRotator, 0.5f, 1.0f);
	
}

void AAB_VoidSlam::PlayingAbility() {
	Super::PlayingAbility();
}

void AAB_VoidSlam::EndAbility() {
	Super::EndAbility();

	WorldRef->GetTimerManager().ClearTimer(ArrowSpawnTimerHandle);
}

void AAB_VoidSlam::SpawnArrows() {
	if (!IsValid(ProjectileClass)) return;
	
	// Generate a random point in sphere
	float RandomAngle = FMath::RandRange(0.0f, 2 * PI);
	float RandomRadius = FMath::RandRange(0.0f, RangeCastDistance);
	float XOffset = RandomRadius * FMath::Cos(RandomAngle);
	float YOffset = RandomRadius * FMath::Sin(RandomAngle);

	FVector SpawnLocation = CastLocation + FVector(XOffset, YOffset, 1000);
	FRotator SpawnRotation = FRotator(-90, 0, 0);
	
	// Spawn the projectile
	AProjectileBase* SpawnedProjectile = WorldRef->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, FActorSpawnParameters());
	SpawnedProjectile->Damage = 0;
	SpawnedProjectile->ProjectileMovement->Velocity *= 2000;
	SpawnedProjectile->SetOwner(this->GetOwner());
	
}
