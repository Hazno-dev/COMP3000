// Callum Stables, 2023


#include "AI/Abilities/EnemyBaseAbilities.h"

#include "AI/BaseAICharacter.h"
#include "Heroes/Abilities/AbilityData.h"

// Sets default values for this component's properties
UEnemyBaseAbilities::UEnemyBaseAbilities()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UEnemyBaseAbilities::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() == nullptr) return;
	if (AbilitiesArray.Num() == 0) return;
	if (!Cast<ABaseAICharacter>(GetOwner())) return;
	
	AICharacterRef = Cast<ABaseAICharacter>(GetOwner());

	//gengine aicharacterref
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AICharacterRef: %s"), *AICharacterRef->GetName()));
	
	for (auto Ability : AbilitiesArray) {
		Ability.GetDefaultObject()->CustomBeginPlay(AICharacterRef, GetWorld());
	}
}


// Called every frame
void UEnemyBaseAbilities::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto Ability : AbilitiesArray) {
		Ability.GetDefaultObject()->CustomTick(DeltaTime);
	}
}

void UEnemyBaseAbilities::StartAbility(AEAbilityBase* Ability) {
	Ability->CoreBeginAbility();
}

//Returns an ability that matches the class if it exists
AEAbilityBase* UEnemyBaseAbilities::AbilityFromSubclass(const TSubclassOf<AEAbilityBase> AbilityClass) const {
	for (auto Ability : AbilitiesArray) {
		if (Ability == AbilityClass) return Ability.GetDefaultObject();
	}
	return nullptr;
}

//Returns an ability that matches the type and has charges if it exists
AEAbilityBase* UEnemyBaseAbilities::AbilityFromType(const EAbilityType AbilityType) const {
	for (auto Ability : AbilitiesArray) {
		if (Ability.GetDefaultObject()->AbilityType == AbilityType
			&& Ability.GetDefaultObject()->CurrentCharges > 0) return Ability.GetDefaultObject();
	}
	return nullptr;
}
