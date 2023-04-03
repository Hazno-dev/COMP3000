// Callum Stables, 2023


#include "AI/Abilities/AEB_SummonRitual.h"
#include "NiagaraFunctionLibrary.h"

#include "AI/BaseAICharacter.h"
#include "Helpers/TraceHelpers.h"

void AAEB_SummonRitual::BeginAbility() {
	Super::BeginAbility();

	if (!IsValid(SummonSelectionEQS) || !IsValid(SummonedActor) || SummonCount == 0) return;

	FVector GroundLocation = UTraceHelpers::TraceGroundLocation(AICharacterRef, 100.f, 1000.f);
	if (GroundLocation == FVector::ZeroVector) GroundLocation = AICharacterRef->GetActorLocation();

	if (IsValid(SummonVFX)) 
			SpawnedVFX.Add(UNiagaraFunctionLibrary::SpawnSystemAtLocation
			(WorldRef, SummonVFX, GroundLocation, FRotator::ZeroRotator, FVector::OneVector));
	
	if (IsValid(SummonMontage)) AICharacterRef->PlayAnimMontage(SummonMontage);
	
	if (const auto PlayerRef = WorldRef->GetFirstPlayerController()->GetPawn())
		QueryRequest = FEnvQueryRequest(SummonSelectionEQS, PlayerRef);

	FQueryFinishedSignature QueryFinishedDelegate;
	QueryFinishedDelegate.BindLambda([this](TSharedPtr<FEnvQueryResult> Result) {
		if (Result->IsSuccessful()) {
			SetSummonLocations(Result);
			
			if (!bIsAbilityPlaying && !bIsInterrupted) Summon();
		}
	});

	QueryRequest.Execute(EEnvQueryRunMode::AllMatching, QueryFinishedDelegate);
}

void AAEB_SummonRitual::PlayingAbility() {
	Super::PlayingAbility();
}

void AAEB_SummonRitual::EndAbility() {
	Super::EndAbility();

	Summon();
}

void AAEB_SummonRitual::InterruptAbility() {
	Super::InterruptAbility();

	for (UNiagaraComponent* VFX : SpawnedVFX) {
		if (IsValid(VFX)) {
			VFX->Deactivate();
		}
	}
}


void AAEB_SummonRitual::SetSummonLocations(TSharedPtr<FEnvQueryResult> Result) {
	if (!Result->IsSuccessful()) return;
	
	for (int i = 0; i < SummonCount; i++) {
		if (SummonLocations.Num() >= SummonCount) break;
		
		if (Result->GetItemAsLocation(i) != FVector::ZeroVector)
			SummonLocations.Add(Result->GetItemAsLocation(i));
	}
	
}

void AAEB_SummonRitual::Summon() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Summoning %d actors"), SummonLocations.Num()));
	if (SummonLocations.Num() == 0) return;

	for (int i = 0; i < SummonLocations.Num(); i++) {
		if (IsValid(SummonedActor)) {
			
			ABaseAICharacter* SpawnedActor = WorldRef->SpawnActor<ABaseAICharacter>(SummonedActor, SummonLocations[i], FRotator::ZeroRotator);
			
			if (IsValid(SpawnedActor)) {
				
				SpawnedActor->SetInstigator(AICharacterRef);
				const APlayerController* PlayerController = WorldRef->GetFirstPlayerController();
				
				if (IsValid(PlayerController)) {
					
					APawn* PlayerPawn = PlayerController->GetPawn();
					if (IsValid(PlayerPawn)) SpawnedActor->SetTarget(PlayerPawn);
				}
			}
			
			if (IsValid(SummonedActorVFX)) SpawnedVFX.Add( UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldRef,
				SummonedActorVFX, SummonLocations[i], FRotator::ZeroRotator, FVector::OneVector));
		}
	}

	SummonLocations.Empty();
}

