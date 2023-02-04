// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FacePlayer.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UBTTask_FacePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
