// Callum Stables, 2023


#include "AI/Service/BTService_UpdateTargetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GameFramework/Actor.h"

UBTService_UpdateTargetLocation::UBTService_UpdateTargetLocation()
{
	bNotifyTick = true;
	TickInterval = 0.5f; // Set the tick interval to 0.5 seconds
}

void UBTService_UpdateTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;
	

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!TargetActor) return;
	

	FVector TargetLocation = TargetActor->GetActorLocation();
	BlackboardComp->SetValueAsVector("TargetLocation", TargetLocation); 
}
