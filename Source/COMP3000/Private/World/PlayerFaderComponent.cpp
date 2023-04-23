// Callum Stables, 2023


#include "World/PlayerFaderComponent.h"

#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPlayerFaderComponent::UPlayerFaderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TickRate = 0.09;
	TraceSize = 50.0;
	TraceType = UEngineTypes::ConvertToTraceType(ECC_Camera);
	DebugTraceType = EDrawDebugTrace::ForDuration;
}


// Called when the game starts
void UPlayerFaderComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickInterval(TickRate);
	SetComponentTickEnabled(true);

	Player = Cast<AMainCharacter>(GetOwner());
	if (IsValid(Player)) {
		PlayerCamera = Player->GetCamera();
		if (DebugFadeTrace) DebugTraceType = EDrawDebugTrace::ForDuration;
		else DebugTraceType = EDrawDebugTrace::None;
	}
}


// Called every frame
//Trace between player and camera, detecting objects in the way, if they have a fader component it will fade the meshes.
void UPlayerFaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(Player)) return;
	if (PlayerCamera == nullptr) return;

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector PlayerCameraLocation = PlayerCamera->GetComponentLocation();
	const FVector DirectionUnitVector = UKismetMathLibrary::GetDirectionUnitVector(PlayerLocation, PlayerCameraLocation);
	
	FVector TraceEnd = PlayerLocation + ((TraceDistanceFromPlayer + TraceSize) * DirectionUnitVector);
	TraceEnd.Z += TraceHeightOffset;

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(Player);

	TArray<FHitResult> HitResults;

	//Trace from player to camera
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), TraceEnd, PlayerCameraLocation,
		TraceSize, TraceType, true, IgnoredActors, DebugTraceType, HitResults,
		true, FLinearColor::Red, FLinearColor::Green, 0.09f);

	//stores faders that need to be updated based on the trace results and angle checks
	TArray<UMeshFaderComponent*> CurrentFadersToUpdate;

	//foreach hitresult check if it has a fader component, if so add it to the array
	if (HitResults.Num() > 0) {
		for (FHitResult HitResult : HitResults) {
			if (!IsValid(HitResult.GetActor())) continue;
			if (UMeshFaderComponent* MeshFader = HitResult.GetActor()->FindComponentByClass<UMeshFaderComponent>()) {
				CurrentFadersToUpdate.Add(MeshFader);
			}
		}
	}

	//Check angle between player forward/direction to object, below difference = still in view regardless of trace.
	/*for (UMeshFaderComponent* CurrentFader : CurrentFaders) {
		AActor* FaderActor = CurrentFader->GetOwner();
		if (IsValid(FaderActor)) {
			//Normalized direction from player to the object
			FVector DirectionToActor = (FaderActor->GetActorLocation() - PlayerLocation).GetSafeNormal();
			float AngleDifference = FMath::Acos(FVector::DotProduct(Player->GetActorForwardVector(), DirectionToActor)) * (180.0f / PI);
			
			if (AngleDifference < 30.0f) {
				CurrentFadersToUpdate.AddUnique(CurrentFader);
			}
		}
	}*/
	
	if (CurrentFadersToUpdate == CurrentFaders) return;

	for (UMeshFaderComponent* CurrentFader : CurrentFadersToUpdate) {
		if (CurrentFaders.Contains(CurrentFader)) continue;
		if (!IsValid(CurrentFader)) continue;

		CurrentFader->FadeIn();
		CurrentFaders.Add(CurrentFader);
	}
	for (UMeshFaderComponent* CurrentFader : CurrentFaders) {
		if (CurrentFadersToUpdate.Contains(CurrentFader)) continue;
		if (!IsValid(CurrentFader)) continue;
		
		CurrentFader->FadeOut();
		CurrentFaders.Remove(CurrentFader);
	}
	
}

