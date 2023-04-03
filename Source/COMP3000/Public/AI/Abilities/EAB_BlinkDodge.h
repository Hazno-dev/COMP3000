// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "AI/Abilities/EAbilityBase.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EAB_BlinkDodge.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API AEAB_BlinkDodge : public AEAbilityBase
{
	GENERATED_BODY()

public:
	virtual void BeginAbility() override;

	virtual void PlayingAbility() override;

	virtual void EndAbility() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	UNiagaraSystem* BlinkVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	UAnimMontage* BlinkMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "EQS")
	UEnvQuery* RandomNearPlayerEQS;

private:
	UPROPERTY()
	FVector PositionToMoveTo;

	UPROPERTY()
	FEnvQueryRequest QueryRequest;

	UFUNCTION()
	void Teleport();
	
};
