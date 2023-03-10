// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Heroes/GAS/GASData.h"
#include "DashGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UDashGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDashGameplayAbility();

	//Activate the ability
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	EPlayerAbilityInputID DashAbilityInputID {EPlayerAbilityInputID::None};
	
	
};
