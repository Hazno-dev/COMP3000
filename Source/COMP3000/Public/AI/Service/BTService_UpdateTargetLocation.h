// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UBTService_UpdateTargetLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_UpdateTargetLocation();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float TickInterval;
	
};
