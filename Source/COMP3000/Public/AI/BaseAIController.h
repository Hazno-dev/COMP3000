// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();

	UFUNCTION()
	void ReevaluateCurrentBTTask();

	UFUNCTION()
	UBehaviorTreeComponent* GetAIBehaviourTreeComponent() const { if(IsValid(AIBehaviourTreeComponent)) return AIBehaviourTreeComponent; return nullptr; }
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> AIBehaviourTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTreeComponent> AIBehaviourTreeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardComponent> AIBlackboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	

	
};
