// Fill out your copyright notice in the Description page of Project Settings.


#include "Heroes/CameraDynamicMotion.h"

#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UCameraDynamicMotion::UCameraDynamicMotion()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CameraLookMax = 10.f;
	CameraZoomMaxFOV = 95.f;
	CameraZoomMinFOV = 0.f;
	CameraZoomTargetFOV = 0.f;
	CameraZoomSpeed = 1.f;

	MaxSpringArmLength = 500.f;
	MinSpringArmLength = 0.f;

	DefaultCameraLag = 0.f;
}


// Called when the game starts
void UCameraDynamicMotion::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner() == nullptr) return;
	
	MainCharacterRef = Cast<AMainCharacter>(GetOwner());
	CameraRef = MainCharacterRef->GetCamera();
	SpringArmRef = MainCharacterRef->GetSpringArm();
	OffsetLocation = MainCharacterRef->GetActorLocation();
	CameraZoomMinFOV = CameraRef->FieldOfView;

	MinSpringArmLength = SpringArmRef->TargetArmLength;
	MaxSpringArmLength = MinSpringArmLength + MaxSpringArmLength;

	DefaultCameraLag = SpringArmRef->CameraLagSpeed;
}


// Called every frame
void UCameraDynamicMotion::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MainCharacterRef == nullptr) return;
	
	CameraOffset(DeltaTime);

	MovementFOV(DeltaTime);

	MovementZoom(DeltaTime);

	CameraLagDecay(DeltaTime);
}

void UCameraDynamicMotion::CameraOffset(float DeltaTime) {
	OffsetLocation = MainCharacterRef->GetActorLocation() +
	MainCharacterRef->GetCharacterMovement()->Velocity.GetSafeNormal2D() * CameraLookMax;

	FRotator NewRotation = FMath::RInterpTo(CameraRef->GetComponentRotation(),
		UKismetMathLibrary::FindLookAtRotation(CameraRef->GetComponentLocation(), OffsetLocation), DeltaTime, 10.f);
	
	if (CameraConstraints(NewRotation.Pitch)) CameraRef->SetWorldRotation(FRotator(NewRotation.Pitch, CameraRef->GetComponentRotation().Yaw, CameraRef->GetComponentRotation().Roll));
	 
	
	//gengine newrotation.pitch
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::SanitizeFloat(NewRotation.Pitch));
}

bool UCameraDynamicMotion::CameraConstraints(float NewPitch) {
	if (NewPitch > -45 && NewPitch < -35) return true;
	
	if (NewPitch < -45) {
		CameraRef->SetWorldRotation(FRotator(-45, CameraRef->GetComponentRotation().Yaw, CameraRef->GetComponentRotation().Roll));
		CameraZoomTargetFOV = CameraZoomMaxFOV;
		CameraZoomSpeed = 5.f;
	} else if (NewPitch > -35) {
		CameraRef->SetWorldRotation(FRotator(-35, CameraRef->GetComponentRotation().Yaw, CameraRef->GetComponentRotation().Roll));
		CameraZoomTargetFOV = CameraZoomMaxFOV;
		CameraZoomSpeed = 5.f;

	}
	return false;
}

void UCameraDynamicMotion::MovementFOV(float DeltaTime) {
	if (MainCharacterRef == nullptr) return;

	CameraRef->SetFieldOfView(FMath::FInterpTo(CameraRef->FieldOfView, CameraZoomTargetFOV, DeltaTime, CameraZoomSpeed));
	
	if (MainCharacterRef->GetCharacterMovement()->Velocity.Size() > 0) {
		CameraZoomTargetFOV = CameraZoomMaxFOV;
		CameraZoomSpeed = 2.f;
	} else {
		CameraZoomTargetFOV = CameraZoomMinFOV;
		CameraZoomSpeed = 4.f;
	}
}

void UCameraDynamicMotion::MovementZoom(float DeltaTime) {
	// Define the maximum distance from the center of the screen
	float MaxDistanceFromCenter = 500.0f;

	// Get the player's location on the screen in pixel coordinates
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector2D ScreenLocation;
	PlayerController->ProjectWorldLocationToScreen(MainCharacterRef->GetActorLocation(), ScreenLocation);

	// Calculate the distance of the player from the center of the screen
	const FVector2D ScreenSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const FVector2D ScreenCenter = ScreenSize / 2.0f;
	float DistanceFromCenter = FVector2D::Distance(ScreenLocation, ScreenCenter);

	// Clamp the distance to the maximum distance
	DistanceFromCenter = FMath::Clamp(DistanceFromCenter, 0.0f, MaxDistanceFromCenter);

	//print distance from center
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::SanitizeFloat(DistanceFromCenter));

	// Adjust the length of the spring arm based on the distance from the center
	float SpringArmLength = FMath::Lerp(MinSpringArmLength, MaxSpringArmLength, DistanceFromCenter / MaxDistanceFromCenter);
	SpringArmRef->TargetArmLength = FMath::FInterpTo(SpringArmRef->TargetArmLength, SpringArmLength, DeltaTime, 5.f);

}

void UCameraDynamicMotion::CameraLagDecay(float DeltaTime) {
	if (MainCharacterRef == nullptr ||
		UKismetMathLibrary::NearlyEqual_FloatFloat(SpringArmRef->CameraLagSpeed, DefaultCameraLag, 0.001)) return;

	SpringArmRef->CameraLagSpeed = FMath::FInterpTo(SpringArmRef->CameraLagSpeed, DefaultCameraLag, DeltaTime, 1.0f);
}
