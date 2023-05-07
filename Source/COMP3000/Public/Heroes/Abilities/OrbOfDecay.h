// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "AI/ProjectileBase.h"
#include "Heroes/Abilities/AbilityBase.h"
#include "World/SplashDamageContainer.h"
#include "OrbOfDecay.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API AOrbOfDecay : public AAbilityBase
{
	GENERATED_BODY()

public:
	
	virtual void BeginAbility() override;

	virtual void PlayingAbility() override;

	virtual void EndAbility() override;

	UFUNCTION()
	void ProjectileDied();

	UFUNCTION()
	void ExplosionSD();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	TSubclassOf<ASplashDamageContainer> SplashDamageContainer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	UNiagaraSystem* Explosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* ExplosionAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* ThrowSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* ThrowAttenuation;

private:
	bool bExplosionStarted = false;
	AProjectileBase* SpawnedProjectile;

	FVector ProjectileDeathLocation;

	FTimerHandle EndProjectileTimerHandle;
};
