// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "AI/Abilities/EAbilityBase.h"
#include "Heroes/Abilities/StatusEffectBase.h"
#include "Sound/SoundCue.h"
#include "EAB_Charge.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API AEAB_Charge : public AEAbilityBase
{
	GENERATED_BODY()

public:
	virtual void BeginAbility() override;

	virtual void PlayingAbility() override;

	virtual void EndAbility() override;

	virtual void InterruptAbility() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	float ChargeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	TSubclassOf<UStatusEffectBase> StunnedStatusEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* ChargeStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* ChargeStartAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<USoundCue*> ChargeFootsteps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* ChargeFootstepsAttenuation;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:

	UFUNCTION()
	void ChargeEnd();
	
	float NormalMoveSpeed;

	AActor* TargetActor;

	FVector ChargeDirection;
	
	class ABaseAIController* AIControllerRef;
	
	bool bOriginalRVOEnabled;
	float OriginalAvoidanceWeight;

	float FootstepCounter = 0;
	
};
