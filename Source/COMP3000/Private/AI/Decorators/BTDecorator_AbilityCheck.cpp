// Callum Stables, 2023

#include "AI/Decorators/BTDecorator_AbilityCheck.h"

#include "AI/BaseAICharacter.h"
#include "AIController.h"

bool UBTDecorator_AbilityCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {

	// Check if the ability is valid
	if (AbilityToCheck == nullptr) return false;
	
	// Check if the AI character is valid
	const ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (AICharacter == nullptr) return false;

	// Check if the character has the ability
	const AEAbilityBase* Ability = AICharacter->EnemyBaseAbilitiesComponent->AbilityFromSubclass(AbilityToCheck);
	if (Ability == nullptr) return false;

	// Check if the ability has charges
	if (Ability->CurrentCharges > 0) return true;

	return false;
	
}
