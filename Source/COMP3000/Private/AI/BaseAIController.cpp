// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/Engine.h"
#include "Perception/AIPerceptionComponent.h"

ABaseAIController::ABaseAIController() {

	//tick enabled
	PrimaryActorTick.bCanEverTick = true;
	
	AIBehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("AIBehaviourTreeComponent"));
	AIBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("AIBlackboard"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
}

void ABaseAIController::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(AIBehaviourTree.Get())) {

		RunBehaviorTree((AIBehaviourTree.Get()));
		AIBehaviourTreeComponent->StartTree(*AIBehaviourTree.Get());
		
	} else  UE_LOG(LogTemp, Error, TEXT("AIBehaviourTree is not set"));
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnTargetPerceptionUpdated);
}

void ABaseAIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	if (IsValid(AIBlackboard.Get()) && IsValid(AIBehaviourTree.Get())) {
		AIBlackboard->InitializeBlackboard(*(AIBehaviourTree.Get()->BlackboardAsset));
	} else UE_LOG(LogTemp, Error, TEXT("AIBlackboard/AIBehaviourTree is not set"));
}

void ABaseAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) {
	Blackboard->SetValueAsObject("ActorTarget", Actor);
}
