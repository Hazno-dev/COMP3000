// Callum Stables, 2023


#include "Heroes/Abilities/OrbOfDecay.h"

#include "MainCharacter.h"
#include "Helpers/TraceHelpers.h"
#include "Kismet/GameplayStatics.h"

void AOrbOfDecay::BeginAbility() {
	Super::BeginAbility();

	if (!IsValid(ProjectileClass)) return;

	FRotator Direction = MainCharacterRef->GetActorRotation();
	FVector Location = MainCharacterRef->GetActorLocation() + (Direction.Vector() * 100.0f);

	SpawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, Location , Direction, FActorSpawnParameters());

	if (!IsValid(SpawnedProjectile)) return;

	if (ThrowSound && ThrowAttenuation) UGameplayStatics::PlaySoundAtLocation(WorldRef, ThrowSound, Location, FRotator::ZeroRotator, 0.5f, 1.0f, 0.0f, ThrowAttenuation);
	else if (ThrowSound) UGameplayStatics::PlaySoundAtLocation(WorldRef, ThrowSound, Location, FRotator::ZeroRotator, 0.5f, 1.0f);
	
	SpawnedProjectile->Damage = Damage;
	SpawnedProjectile->ProjectileMovement->Velocity *= 2000;
	SpawnedProjectile->SetOwner(this->GetOwner());
	SpawnedProjectile->OnProjectileHit.AddDynamic(this, &AOrbOfDecay::ProjectileDied);
}

void AOrbOfDecay::PlayingAbility() {
	Super::PlayingAbility();
}

void AOrbOfDecay::EndAbility() {
	Super::EndAbility();
	
}

void AOrbOfDecay::ProjectileDied() {
	if (!IsValid(SplashDamageContainer)) return;
	if (!IsValid(SpawnedProjectile)) return;
	if (!IsValid(Explosion)) return;
	
	ProjectileDeathLocation = SpawnedProjectile->GetActorLocation();
	
	FVector SpawnVFXLoc = UTraceHelpers::TraceGroundLocation(SpawnedProjectile, 100, 1000.f);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldRef, Explosion, SpawnVFXLoc);

	if (ExplosionSound && ExplosionAttenuation) UGameplayStatics::PlaySoundAtLocation(WorldRef, ExplosionSound, SpawnVFXLoc, FRotator::ZeroRotator, 0.5f, 1.0f, 0.0f, ExplosionAttenuation);
	else if (ExplosionSound) UGameplayStatics::PlaySoundAtLocation(WorldRef, ExplosionSound, SpawnVFXLoc, FRotator::ZeroRotator, 0.5f, 1.0f);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOrbOfDecay::ExplosionSD, 0.7f, false);
}

void AOrbOfDecay::ExplosionSD() {
	ASplashDamageContainer* SpawnedSplash = WorldRef->SpawnActor<ASplashDamageContainer>(SplashDamageContainer, ProjectileDeathLocation,
FRotator::ZeroRotator, FActorSpawnParameters());
	SpawnedSplash->SetOwner(GetOwner());
	
}
