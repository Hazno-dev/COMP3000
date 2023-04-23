// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_RangeCheck.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

UBTDecorator_RangeCheck::UBTDecorator_RangeCheck()
{
	NodeName = "Range Check";
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_RangeCheck, BlackboardKey));
	BlackboardKey.SelectedKeyType = UBlackboardKeyType_Vector::StaticClass(); 
}


bool UBTDecorator_RangeCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn = AIController->GetPawn();
	const FVector Origin = AIPawn->GetActorLocation();

	// Get target location from the Blackboard
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	FVector TargetLocation = BlackboardComp->GetValueAsVector(GetSelectedBlackboardKey());
	
	if (FVector::Dist(Origin, TargetLocation) >= MinDistance && FVector::Dist(Origin, TargetLocation) <= MaxDistance)
	{
		return true;
	}

	return false;
}
