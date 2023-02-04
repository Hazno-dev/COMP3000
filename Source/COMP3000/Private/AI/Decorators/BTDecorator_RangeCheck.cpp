// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_RangeCheck.h"

#include "AIController.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

bool UBTDecorator_RangeCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn = AIController->GetPawn();
	const FVector Origin = AIPawn->GetActorLocation();
	const FVector PlayerLocation = Cast<APlayerController>(GetWorld()->GetFirstPlayerController())->GetPawn()->GetActorLocation();

	if (FVector::Dist(Origin, PlayerLocation) >= MinDistance && FVector::Dist(Origin, PlayerLocation) <= MaxDistance) {
		return true;
	}

	return false;
}
