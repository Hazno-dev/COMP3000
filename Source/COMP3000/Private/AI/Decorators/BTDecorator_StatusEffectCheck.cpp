// Callum Stables, 2023


#include "AI/Decorators/BTDecorator_StatusEffectCheck.h"

#include "AIController.h"
#include "AI/BaseAICharacter.h"

bool UBTDecorator_StatusEffectCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                uint8* NodeMemory) const {

	ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(AICharacter)) return false;

	for (const EStatusEffect StatusEffect : StatusEffectsToCheck) {
		if (!AICharacter->EnemyStatusEffectSystemComponent->GetStatusEffectsOfType(StatusEffect).IsEmpty()) return false;
	}
	
	return true;
}
