// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_FacePlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTTask_FacePlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	FRotator Destination;
	AIController->GetBlackboardComponent()->GetRotationFromEntry(BlackboardKey.SelectedKeyName, Destination);
	
	FRotator InterpValue = FMath::RInterpConstantTo(AIController->GetPawn()->GetActorRotation(), Destination, DeltaSeconds, 100.0f);

	AIController->GetPawn()->SetActorRotation(InterpValue);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
