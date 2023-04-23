// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PlayMontage.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UBTTask_PlayMontage : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_PlayMontage();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage;

	UFUNCTION()
	void FinishTask();

	UPROPERTY()
	class ABaseAICharacter* AICharacterRef;
};
