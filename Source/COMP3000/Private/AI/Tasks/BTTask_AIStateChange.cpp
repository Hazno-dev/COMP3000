// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_AIStateChange.h"

#include "AIController.h"
#include "AI/BaseAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
EBTNodeResult::Type UBTTask_AIStateChange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	
	ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(OwnerComp.GetAIOwner()->GetCharacter());

	if (AICharacter == nullptr) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
	
	AICharacter->SetAIState(AIStateInput);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
