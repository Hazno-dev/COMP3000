// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_AbilityCheckTick.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UBTService_AbilityCheckTick : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_AbilityCheckTick();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bCheckRange = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float MinDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float MaxDistance = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEAbilityBase> AbilityToCheck;*/
};
