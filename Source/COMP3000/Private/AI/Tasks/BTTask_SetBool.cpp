// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_SetBool.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetBool::UBTTask_SetBool() {
	NodeName = "Set Bool";

	BlackboardKey.AddBoolFilter(this,GET_MEMBER_NAME_CHECKED(UBTTask_SetBool, BlackboardKey));
}

EBTNodeResult::Type UBTTask_SetBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BlackboardKey.SelectedKeyName, BoolInput);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
