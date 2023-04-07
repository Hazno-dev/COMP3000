// Callum Stables, 2023


#include "Heroes/Abilities/AB_Dash.h"

#include "NiagaraFunctionLibrary.h"
#include "MainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Heroes/HeroGenerator.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Heroes/CameraDynamicMotion.h"

void AAB_Dash::BeginAbility() {
	Super::BeginAbility();
	
	if (MainCharacterRef == nullptr) return;

	MainCharacterRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	MainCharacterRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(DashExplosiveVFX[0], MainCharacterRef->GetMesh(), 
"Pelvis", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
	MainCharacterRef->HeroGeneratorComponent->SetMeshesHidden(true);

	for (FDashBones Bone : DashBones) {
		UNiagaraComponent* CreatedDashLine = UNiagaraFunctionLibrary::SpawnSystemAttached(DashTrailVFX[0], MainCharacterRef->GetMesh(),
			Bone.BoneName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
		CreatedDashLine->SetFloatParameter("User.RibbonWidth", Bone.RibbonWidth);
		CreatedDashLine->SetFloatParameter("User.RibbonLenght", Bone.RibbonLength);
		CreatedDashLine->SetFloatParameter("User.Duration", AbilityDuration);
	}

	DashDirection = (MainCharacterRef->GetCharacterMovement()->GetLastInputVector().IsNearlyZero())
		? MainCharacterRef->GetActorForwardVector() : MainCharacterRef->GetCharacterMovement()->GetLastInputVector();

	DashDirection.Z = 0.f;

	MainCharacterRef->Crouch();
	
	MainCharacterRef->GetCharacterMovement()->MaxWalkSpeed = MainCharacterRef->GetCharacterMovement()->MaxWalkSpeed + DashMovementSpeedIncrease;
	MainCharacterRef->GetCharacterMovement()->MaxWalkSpeedCrouched = MainCharacterRef->GetCharacterMovement()->MaxWalkSpeedCrouched + DashMovementSpeedIncrease;
	MainCharacterRef->GetCharacterMovement()->MaxAcceleration = MainCharacterRef->GetCharacterMovement()->MaxAcceleration + 100000000.f;

	MainCharacterRef->CameraDynamicMotion->SetCameraLag(1.f);

	DisableInput(MainCharacterRef->SavedController);
}

void AAB_Dash::PlayingAbility() {
	Super::PlayingAbility();
	if (MainCharacterRef == nullptr) return;
	MainCharacterRef->AddMovementInput(DashDirection, 5.f);
	
}

void AAB_Dash::EndAbility() {
	Super::EndAbility();

	if (MainCharacterRef == nullptr) return;

	MainCharacterRef->UnCrouch();
	
	MainCharacterRef->HeroGeneratorComponent->SetMeshesHidden(false);
	UNiagaraFunctionLibrary::SpawnSystemAttached(DashExplosiveVFX[0], MainCharacterRef->GetMesh(), 
"Pelvis", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

	MainCharacterRef->GetCharacterMovement()->MaxWalkSpeed = BaseCharacterSpeed;
	MainCharacterRef->GetCharacterMovement()->MaxWalkSpeedCrouched = BaseCharacterSpeed;
	MainCharacterRef->GetCharacterMovement()->MaxAcceleration = BaseCharacterAcceleration;

	EnableInput(MainCharacterRef->SavedController);
	MainCharacterRef->GetCharacterMovement()->StopMovementImmediately();
	
	MainCharacterRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	MainCharacterRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	
}

