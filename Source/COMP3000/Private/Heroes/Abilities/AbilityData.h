#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/StructOnScope.h"
#include "AbilityData.generated.h"


UENUM(BlueprintType)
enum EAbilitySlotType
{
	Dash,
	Ability1,
	Ability2,
	Ability3,
	Ultimate
};


UENUM(BlueprintType)
enum EAbilityType
{
	Damage,
	Heal,
	Shield,
	Ritual,
	Mitigation,
	DeathCry
};

UENUM(BlueprintType)
enum EStatusEffect
{
	Slowness,
	Nimble,
	Stun,
	Root,
	Burning,
	Invincible,
	Berserk,
	Silence,
	VisualOnly
};

USTRUCT(BlueprintType)
struct FStatusEffectData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EStatusEffect> StatusEffect;
	
	UPROPERTY(BlueprintReadWrite)
	float Duration;

	UPROPERTY(BlueprintReadWrite)
	bool bIsPermanent;

	//Strength is defined by 0 - 1, 0 being no effect, 1 being 100% effect etc.
	//Some effects are not affected by strength, such as stun, root, invincible, silence.
	UPROPERTY(BlueprintReadWrite)
	float Strength;

	FStatusEffectData()
	{
		StatusEffect = EStatusEffect::Slowness;
		Duration = 0.0f;
		Strength = 0.0f;
		bIsPermanent = false;
	}

	void Initialize(EStatusEffect NewStatusEffect, float NewDuration, float NewStrength, bool NewIsPermanent)
	{
		StatusEffect = NewStatusEffect;
		Duration = NewDuration;
		Strength = NewStrength;
		bIsPermanent = NewIsPermanent;
	}
};