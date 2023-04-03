// Callum Stables, 2023


#include "AI/Tasks/BTTask_UseAIAbility.h"

#include "AIController.h"
#include "AI/BaseAICharacter.h"

UBTTask_UseAIAbility::UBTTask_UseAIAbility() {
	bNotifyTick = true;
	NodeName = "Use AI Ability";
	Ability = nullptr;
}

EBTNodeResult::Type UBTTask_UseAIAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	// Check if the ability is valid
	if (!IsValid(AbilityToUse)) return EBTNodeResult::Failed;

	// Check if the AI character is valid
	const ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(AICharacter)) return EBTNodeResult::Failed;

	// Check if the character has the ability
	Ability = AICharacter->EnemyBaseAbilitiesComponent->AbilityFromSubclass(AbilityToUse);
	if (!IsValid(Ability)) return EBTNodeResult::Failed;

	if (Ability->bIsAbilityPlaying) return EBTNodeResult::Failed;
	
	// Start the ability
	Ability->CoreBeginAbility();
	return EBTNodeResult::InProgress;
}

void UBTTask_UseAIAbility::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	// Check if the ability is valid
	if (Ability->bIsAbilityPlaying) return;

	// Finish the task
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}


