// Callum Stables, 2023


#include "AI/Abilities/EAB_BlinkDodge.h"

#include "NavigationSystem.h"
#include "AI/BaseAICharacter.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "NiagaraFunctionLibrary.h"

void AEAB_BlinkDodge::BeginAbility() {
	Super::BeginAbility();
	
	PositionToMoveTo = FVector::ZeroVector;

	if (!IsValid(RandomNearPlayerEQS)) return;
	
	if (IsValid(BlinkVFX)) UNiagaraFunctionLibrary::SpawnSystemAtLocation
		(WorldRef, BlinkVFX, AICharacterRef->GetActorLocation(), FRotator::ZeroRotator, FVector::OneVector);
	if (IsValid(BlinkMontage) && !AICharacterRef->GetCurrentMontage()) AICharacterRef->PlayAnimMontage(BlinkMontage);
	
	QueryRequest = FEnvQueryRequest(RandomNearPlayerEQS, WorldRef->GetFirstPlayerController()->GetPawn());
	AICharacterRef->AIState = EAIState::Chase;
	
	FQueryFinishedSignature QueryFinishedDelegate;
	QueryFinishedDelegate.BindLambda([this](TSharedPtr<FEnvQueryResult> Result) {
		if (Result->IsSuccessful()) {
			PositionToMoveTo = Result->GetItemAsLocation(0);
			
			if (!bIsAbilityPlaying) Teleport();
		}
	});

	QueryRequest.Execute(EEnvQueryRunMode::SingleResult, QueryFinishedDelegate);
}


void AEAB_BlinkDodge::PlayingAbility() {
	Super::PlayingAbility();
}

void AEAB_BlinkDodge::EndAbility() {
	Super::EndAbility();

	if (PositionToMoveTo != FVector::ZeroVector) Teleport();
}

void AEAB_BlinkDodge::Teleport() {
	AICharacterRef->SetActorLocation(PositionToMoveTo, false);
	if (IsValid(BlinkVFX)) UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldRef, BlinkVFX, AICharacterRef->GetActorLocation(), FRotator::ZeroRotator, FVector::OneVector);
}



