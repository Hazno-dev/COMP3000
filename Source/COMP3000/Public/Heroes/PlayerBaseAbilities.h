// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/ActorComponent.h"
#include "Heroes/Abilities/AB_Dash.h"
#include "Heroes/Abilities/AbilityData.h"
#include "PlayerBaseAbilities.generated.h"

class AMainCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMP3000_API UPlayerBaseAbilities : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerBaseAbilities();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	TSubclassOf<AAB_Dash> DashClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability 1")
	TSubclassOf<AAbilityBase> Ability1Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability 2")
	TSubclassOf<AAbilityBase> Ability2Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability 3")
	TSubclassOf<AAbilityBase> Ability3Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability 4")
	TSubclassOf<AAbilityBase> UltimateClass;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void StartDash();

	UFUNCTION()
	void HeldDash();

	UFUNCTION()
	void EndDash();

	UFUNCTION()
	UTexture2D* AbilityTextureFromEnum(const EAbilitySlotType AbilitySlotType) const;

private:

	UPROPERTY()
	AMainCharacter* MainCharacterRef;
};
