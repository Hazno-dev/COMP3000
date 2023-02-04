// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_FindRandomPos.h"

#include "NavigationSystem.h"
#include "AIController.h"
#include "AI/BaseAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomPos::UBTTask_FindRandomPos() {
	NodeName = "Find Random Pos";

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomPos, BlackboardKey));
}

FString UBTTask_FindRandomPos::GetStaticDescription() const {
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

EBTNodeResult::Type UBTTask_FindRandomPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	FNavLocation PositionToMoveTo{};

	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn = AIController->GetPawn();

	if (Cast<ABaseAICharacter>(AIPawn) == nullptr) return EBTNodeResult::Failed;
	
	const AAIGroupManager* PawnGroupManager = Cast<ABaseAICharacter>(AIPawn)->GetGroupManager();
	const FVector Origin = AIPawn->GetActorLocation();
	
	const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if(NavSys->GetRandomPointInNavigableRadius(Origin, MoveRadius, PositionToMoveTo)) {
		if (FVector::Dist(PositionToMoveTo.Location, PawnGroupManager->GetActorLocation()) <= PawnGroupManager->SphereRadius) {
			AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, PositionToMoveTo.Location);
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
