// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"

#include "AI/BaseAICharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/Engine.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

ABaseAIController::ABaseAIController() {

	//tick enabled
	PrimaryActorTick.bCanEverTick = true;
	
	AIBehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("AIBehaviourTreeComponent"));
	AIBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("AIBlackboard"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
}

void ABaseAIController::ReevaluateCurrentBTTask() {
	//AIBehaviourTreeComponent->RequestBranchEvaluation(EBTNodeResult::Succeeded);
	AIBehaviourTreeComponent->RestartTree();
}

void ABaseAIController::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(AIBehaviourTree.Get())) {
		RunBehaviorTree((AIBehaviourTree.Get()));
		AIBehaviourTreeComponent->StartTree(*AIBehaviourTree.Get());
	} else {
		UE_LOG(LogTemp, Error, TEXT("AIBehaviourTree is not set"));
	}

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnTargetPerceptionUpdated);
	AIPerceptionComponent->SetComponentTickInterval(0.3f);

	// Set the initial tick interval for the BehaviorTreeComponent
	AIBehaviourTreeComponent->SetComponentTickInterval(0.5f);
	GetPathFollowingComponent()->SetComponentTickInterval(0.5f);
}


void ABaseAIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	if (IsValid(AIBlackboard.Get()) && IsValid(AIBehaviourTree.Get())) {
		AIBlackboard->InitializeBlackboard(*(AIBehaviourTree.Get()->BlackboardAsset));
	} else UE_LOG(LogTemp, Error, TEXT("AIBlackboard/AIBehaviourTree is not set"));
}

void ABaseAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) {
	//Blackboard->SetValueAsObject("ActorTarget", Actor);
	
	ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(GetPawn());
	if (IsValid(AICharacter)) AICharacter->SetTarget(Actor);

	// Disable the perception component after the player is detected
	AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
	AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &ABaseAIController::OnTargetPerceptionUpdated);

	// Update the tick interval for the BehaviorTreeComponent when the player is detected
	AIBehaviourTreeComponent->SetComponentTickInterval(0.0f);
	GetPathFollowingComponent()->SetComponentTickInterval(0.01f);

	
}
