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

EBTNodeResult::Type UBTTask_MoveToEnhanced::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	
	AIController = OwnerComp.GetAIOwner();

	
	const APawn* AIPawn = AIController->GetPawn();
	const FVector Origin = AIPawn->GetActorLocation();
	
	AIController->GetBlackboardComponent()->GetLocationFromEntry(BlackboardKey.SelectedKeyName, Destination);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Destination: %s"), *Destination.ToString()));

	AIController->MoveToLocation(Destination, 1, true, true, true, true);

	AIController->GetPathFollowingComponent()->OnRequestFinished.AddLambda([this, &OwnerComp](FAIRequestID RequestID, const FPathFollowingResult& Result)
	{
		if (Result.IsSuccess())
		{
			OwnerComp.OnTaskFinished(Cast<UBTTaskNode>(this), EBTNodeResult::Succeeded);
		}
		else
		{
			OwnerComp.OnTaskFinished(Cast<UBTTaskNode>(this), EBTNodeResult::Failed);
		}
	});


	
	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	return EBTNodeResult::InProgress;
}


EBTNodeResult::Type UBTTask_MoveToEnhanced::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {


	UE_LOG(LogTemp, Warning, TEXT("Aborting MoveTo"));
	if (!IsValid(AIController)) return EBTNodeResult::Aborted;

	UE_LOG(LogTemp, Warning, TEXT("Aborting MoveTo"));
	
	AIController = OwnerComp.GetAIOwner();
	AIController->StopMovement();

	return EBTNodeResult::Aborted;
}
