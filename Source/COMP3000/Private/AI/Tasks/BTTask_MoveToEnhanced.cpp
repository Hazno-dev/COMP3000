// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_MoveToEnhanced.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/BaseAICharacter.h"
#include "AI/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToEnhanced::UBTTask_MoveToEnhanced() {
	NodeName = "Move To Enhanced";
}

FString UBTTask_MoveToEnhanced::GetStaticDescription() const {
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

EBTNodeResult::Type UBTTask_MoveToEnhanced::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn = AIController->GetPawn();
	const FVector Origin = AIPawn->GetActorLocation();

	//get location of object blackboard key
	
	
	FVector Destination;
	AIController->GetBlackboardComponent()->GetLocationFromEntry(BlackboardKey.SelectedKeyName, Destination);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Destination: %s"), *Destination.ToString()));

	if (FVector::Dist(Origin, Destination) <= 200.0f) {
		AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	AIController->MoveToLocation(Destination);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
