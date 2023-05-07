// Callum Stables, 2023


#include "AI/Abilities/AEB_DeathExplosion.h"

#include "AI/BaseAICharacter.h"
#include "Kismet/GameplayStatics.h"

void AAEB_DeathExplosion::BeginAbility() {
	Super::BeginAbility();
}

void AAEB_DeathExplosion::PlayingAbility() {
	Super::PlayingAbility();
}

void AAEB_DeathExplosion::EndAbility() {
	Super::EndAbility();

	if (!IsValid(SplashDamageContainer)) return;

	if (!IsValid(AICharacterRef)) return;

	if (ExplosionSound && ExplosionAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, AICharacterRef->GetActorLocation(), .25f, 1.0f, 0.0f, ExplosionAttenuation);
	else if (ExplosionSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, AICharacterRef->GetActorLocation(), .25f, 1.0f);
	

	WorldRef->SpawnActor<ASplashDamageContainer>(SplashDamageContainer, AICharacterRef->GetActorLocation(), FRotator::ZeroRotator);
	AICharacterRef->DeathDestroy();
}
