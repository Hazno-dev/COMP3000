// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Heroes/Abilities/AbilityData.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_StatusEffectCheck.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UBTDecorator_StatusEffectCheck : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EStatusEffect>> StatusEffectsToCheck;
	
};
