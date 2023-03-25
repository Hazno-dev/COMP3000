// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Heroes/Abilities/AbilityData.h"
#include "StatusEffectBase.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UStatusEffectBase : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status Effect")
	FStatusEffectData StatusEffect;
	
};
