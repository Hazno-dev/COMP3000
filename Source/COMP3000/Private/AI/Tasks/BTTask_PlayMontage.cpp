// Callum Stables, 2023


#include "AI/Tasks/BTTask_PlayMontage.h"

#include "AIController.h"
#include "AI/BaseAICharacter.h"

UBTTask_PlayMontage::UBTTask_PlayMontage() {
	bNotifyTick = true;
	NodeName = "Play Montage";
	Montage = nullptr;
}

EBTNodeResult::Type UBTTask_PlayMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	if (!IsValid(Montage)) return EBTNodeResult::Failed;

	// Check if the AI character is valid
	AICharacterRef = Cast<ABaseAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(AICharacterRef)) return EBTNodeResult::Failed;

	if (!IsValid(AICharacterRef->GetMesh())) return EBTNodeResult::Failed;

	if (!IsValid(AICharacterRef->GetMesh()->GetAnimInstance())) return EBTNodeResult::Failed;
	
	if (AICharacterRef->GetMesh()->GetAnimInstance()->Montage_IsPlaying(Montage)) return EBTNodeResult::Failed;

	FTimerHandle TimerHandle;
	float TimeForAnim = AICharacterRef->GetMesh()->GetAnimInstance()->Montage_Play(Montage);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBTTask_PlayMontage::FinishTask, TimeForAnim, false); 
	
	return EBTNodeResult::InProgress;
}

void UBTTask_PlayMontage::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsValid(Montage)) return;
	if (!IsValid(AICharacterRef)) return;
	if (!IsValid(AICharacterRef->GetMesh())) return;
	if (!IsValid(AICharacterRef->GetMesh()->GetAnimInstance())) return;

	if (AICharacterRef->GetMesh()->GetAnimInstance()->Montage_IsPlaying(Montage)) return;

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTTask_PlayMontage::FinishTask() {
	if (!IsValid(Montage)) return;
	if (!IsValid(AICharacterRef)) return;
	if (!IsValid(AICharacterRef->GetMesh())) return;
	if (!IsValid(AICharacterRef->GetMesh()->GetAnimInstance())) return;
	
	AICharacterRef->GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, Montage);
}
