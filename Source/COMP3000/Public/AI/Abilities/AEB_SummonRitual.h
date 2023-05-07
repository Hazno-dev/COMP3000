// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "AI/Abilities/EAbilityBase.h"
#include "NiagaraSystem.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Heroes/Abilities/StatusEffectBase.h"
#include "Sound/SoundCue.h"
#include "AEB_SummonRitual.generated.h"

/**
 * 
 */

class ABaseAICharacter;
UCLASS()
class COMP3000_API AAEB_SummonRitual : public AEAbilityBase
{
	GENERATED_BODY()

public:
	virtual void BeginAbility() override;

	virtual void PlayingAbility() override;

	virtual void EndAbility() override;

	virtual void InterruptAbility() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	TSubclassOf<ABaseAICharacter> SummonedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	UNiagaraSystem* SummonedActorVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	UAnimMontage* SummonMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	UNiagaraSystem* SummonVFX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	uint8 SummonCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	TSubclassOf<UStatusEffectBase> SummonStatusEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* SummonSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* SummonAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* LightningSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* LightningAttenuation;

	UPROPERTY(EditDefaultsOnly, Category = "EQS")
	TObjectPtr<UEnvQuery> SummonSelectionEQS;

private:
	UPROPERTY()
	TArray<FVector> SummonLocations;
	FEnvQueryRequest QueryRequest;
	UPROPERTY()
	TArray<UNiagaraComponent*> SpawnedVFX;
	
	void SetSummonLocations(TSharedPtr<FEnvQueryResult> Result);

	UFUNCTION()
	void Summon();

	UStatusEffectBase* SummonStatusEffectRef;
};
