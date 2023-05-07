// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "AI/Abilities/EAbilityBase.h"
#include "NiagaraSystem.h"
#include "World/SplashDamageContainer.h"
#include "AEB_DeathExplosion.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API AAEB_DeathExplosion : public AEAbilityBase
{
	GENERATED_BODY()

public:
	virtual void BeginAbility() override;

	virtual void PlayingAbility() override;

	virtual void EndAbility() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	TSubclassOf<ASplashDamageContainer> SplashDamageContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* ExplosionAttenuation;
	
};
