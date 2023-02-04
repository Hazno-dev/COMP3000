// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToEnhanced.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UBTTask_MoveToEnhanced : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_MoveToEnhanced();

private:
	virtual FString GetStaticDescription() const override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
