// Callum Stables, 2023


#include "AI/Service/BTService_AbilityCheckTick.h"

#include "AIController.h"
#include "AI/BaseAICharacter.h"

UBTService_AbilityCheckTick::UBTService_AbilityCheckTick()
{
	bNotifyTick = true;
}

void UBTService_AbilityCheckTick::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	/*Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!IsValid(AIController)) return;
	
	const APawn* AIPawn = AIController->GetPawn();
	const FVector Origin = AIPawn->GetActorLocation();

	const APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	
	if (!IsValid(PlayerController)) return;
	
	const FVector PlayerLocation = PlayerController->GetPawn()->GetActorLocation();

	bool bInRange = FVector::Dist(Origin, PlayerLocation) >= MinDistance && FVector::Dist(Origin, PlayerLocation) <= MaxDistance;

	const ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(AIPawn);
	if (!AICharacter) return;

	const AEAbilityBase* Ability = AICharacter->EnemyBaseAbilitiesComponent->AbilityFromSubclass(AbilityToCheck);
	bool bAbilityReady = Ability && Ability->CurrentCharges > 0;*/
	
}