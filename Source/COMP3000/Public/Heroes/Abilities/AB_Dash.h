// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Heroes/Abilities/AbilityBase.h"
#include "AB_Dash.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDashBones
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RibbonLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RibbonWidth;
};

UCLASS()
class COMP3000_API AAB_Dash : public AAbilityBase
{
	GENERATED_BODY()

public:
	virtual void BeginAbility() override;

	virtual void PlayingAbility() override;

	virtual void EndAbility() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	TArray<UNiagaraSystem*> DashTrailVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	TArray<UNiagaraSystem*> DashExplosiveVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	TArray<FDashBones> DashBones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashMovementSpeedIncrease;
	
private:
	FVector DashDirection;
	
	
};
