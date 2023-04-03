// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "AI/Abilities/EAbilityBase.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UseAIAbility.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UBTTask_UseAIAbility : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_UseAIAbility();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	AEAbilityBase* Ability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEAbilityBase> AbilityToUse;
};
