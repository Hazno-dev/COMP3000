// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetBool.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UBTTask_SetBool : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_SetBool();
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool BoolInput;
	
};
