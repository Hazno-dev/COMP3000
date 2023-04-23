// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "AI/ProjectileBase.h"
#include "Heroes/Abilities/AbilityBase.h"
#include "World/SplashDamageContainer.h"
#include "AB_VoidSlam.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API AAB_VoidSlam : public AAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginAbility() override;

	virtual void PlayingAbility() override;

	virtual void EndAbility() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	TSubclassOf<ASplashDamageContainer> SplashDamageContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	TSubclassOf<AProjectileBase> ProjectileClass;

private:
	FTimerHandle ArrowSpawnTimerHandle;

	UFUNCTION()
	void SpawnArrows();
	
};
