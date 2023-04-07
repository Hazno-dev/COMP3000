// Callum Stables, 2023


#include "AI/Abilities/AEB_DeathExplosion.h"

#include "AI/BaseAICharacter.h"

void AAEB_DeathExplosion::BeginAbility() {
	Super::BeginAbility();
}

void AAEB_DeathExplosion::PlayingAbility() {
	Super::PlayingAbility();
}

void AAEB_DeathExplosion::EndAbility() {
	Super::EndAbility();

	if (!IsValid(SplashDamageContainer)) return;

	WorldRef->SpawnActor<ASplashDamageContainer>(SplashDamageContainer, AICharacterRef->GetActorLocation(), FRotator::ZeroRotator);
	AICharacterRef->DeathDestroy();
}
