// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "EAbilityBase.h"
#include "Components/ActorComponent.h"
#include "EnemyBaseAbilities.generated.h"

class ABaseAICharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMP3000_API UEnemyBaseAbilities : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyBaseAbilities();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<AEAbilityBase>> AbilitiesArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities Data")
	TSubclassOf<AFadeText> FadeTextClass;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void StartAbility(AEAbilityBase* Ability);
	
	UFUNCTION()
	AEAbilityBase* AbilityFromSubclass(const TSubclassOf<AEAbilityBase> AbilityClass) const;

	UFUNCTION()
	AEAbilityBase* AbilityFromType(const EAbilityType AbilityType) const;

private:

	UPROPERTY()
	ABaseAICharacter* AICharacterRef;

	UPROPERTY()
	TArray<AEAbilityBase*> AbilityInstances;

};
