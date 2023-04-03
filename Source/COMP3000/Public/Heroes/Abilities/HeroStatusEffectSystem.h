// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Heroes/Abilities/AbilityData.h"
#include "HeroStatusEffectSystem.generated.h"

class AMainCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMP3000_API UHeroStatusEffectSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHeroStatusEffectSystem();

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddStatusEffect(FStatusEffectData StatusEffectData);

	UPROPERTY()
	TArray<FStatusEffectData> CurrentStatusEffects;

private:
	AMainCharacter* MainCharacterRef;

	UFUNCTION()
	void OnStatusEffectExpired();
};
