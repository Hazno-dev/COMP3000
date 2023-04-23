// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_GreaterCheck.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

UBTDecorator_GreaterCheck::UBTDecorator_GreaterCheck() {
	NodeName = "Greater Check";
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_GreaterCheck, BlackboardKey));
}

bool UBTDecorator_GreaterCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn = AIController->GetPawn();
	const FVector Origin = AIPawn->GetActorLocation();
	// Get target location from the Blackboard
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	FVector TargetLocation = BlackboardComp->GetValueAsVector(GetSelectedBlackboardKey());
	
	if (FVector::Dist(Origin, TargetLocation) > DistanceCheck) {
		return true;
	}

	return false;
}
