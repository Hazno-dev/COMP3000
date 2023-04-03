// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "AI/Abilities/EAbilityBase.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_AbilityCheck.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UBTDecorator_AbilityCheck : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEAbilityBase> AbilityToCheck;
	
};
