// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_GreaterCheck.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UBTDecorator_GreaterCheck : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_GreaterCheck();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float DistanceCheck = 100.0f;
};
