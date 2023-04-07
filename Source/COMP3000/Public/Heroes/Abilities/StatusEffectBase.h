// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Heroes/Abilities/AbilityData.h"
#include "NiagaraSystem.h"
#include "StatusEffectBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class COMP3000_API UStatusEffectBase : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	TEnumAsByte<EStatusEffect> StatusEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	bool bIsPermanent;

	//Strength is defined by 0 - 1, 0 being no effect, 1 being 100% effect etc.
	//Some effects are not affected by strength, such as stun, root, invincible, silence.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	float Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	UTexture2D* StatusEffectIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	UNiagaraSystem* StatusEffectVFX;

	UFUNCTION()
	void SetDuration (float InDuration) { Duration = InDuration; };

	UFUNCTION()
	void SetStrength (float InStrength) { Strength = InStrength; };
};
