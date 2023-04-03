// Callum Stables, 2023

#include "Heroes/Abilities/HeroStatusEffectSystem.h"
//#include "MainCharacter.h"

// Sets default values for this component's properties
UHeroStatusEffectSystem::UHeroStatusEffectSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHeroStatusEffectSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHeroStatusEffectSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHeroStatusEffectSystem::AddStatusEffect(FStatusEffectData StatusEffectData) {
	CurrentStatusEffects.Add(StatusEffectData);
	// Start a timer for the status effect
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UHeroStatusEffectSystem::OnStatusEffectExpired, StatusEffectData.Duration, false);
}

void UHeroStatusEffectSystem::OnStatusEffectExpired() {
	int32 ExpiredEffectIndex = CurrentStatusEffects.IndexOfByPredicate([](const FStatusEffectData& EffectData) {
	return EffectData.Duration <= 0;
});

	// If an expired effect is found, remove it
	if (ExpiredEffectIndex != INDEX_NONE)
	{
		CurrentStatusEffects.RemoveAt(ExpiredEffectIndex);
	}
}



