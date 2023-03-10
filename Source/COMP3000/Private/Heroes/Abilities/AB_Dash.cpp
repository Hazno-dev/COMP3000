// Callum Stables, 2023


#include "Heroes/Abilities/AB_Dash.h"

#include "NiagaraFunctionLibrary.h"
#include "MainCharacter.h"
#include "Heroes/HeroGenerator.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Heroes/CameraDynamicMotion.h"

void AAB_Dash::BeginAbility() {
	Super::BeginAbility();
	
	if (MainCharacterRef == nullptr) return;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Dash");
	
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

	DashDirection = (MainCharacterRef->GetCharacterMovement()->Velocity.GetSafeNormal2D().IsNearlyZero())
		? MainCharacterRef->GetActorForwardVector() : MainCharacterRef->GetCharacterMovement()->Velocity.GetSafeNormal();

	DashDirection.Z = 0.f;
	
	MainCharacterRef->GetCharacterMovement()->MaxWalkSpeed = MainCharacterRef->GetCharacterMovement()->MaxWalkSpeed + DashMovementSpeedIncrease;
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
	
	MainCharacterRef->HeroGeneratorComponent->SetMeshesHidden(false);
	UNiagaraFunctionLibrary::SpawnSystemAttached(DashExplosiveVFX[0], MainCharacterRef->GetMesh(), 
"Pelvis", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

	MainCharacterRef->GetCharacterMovement()->MaxWalkSpeed = BaseCharacterSpeed;
	MainCharacterRef->GetCharacterMovement()->MaxAcceleration = BaseCharacterAcceleration;

	EnableInput(MainCharacterRef->SavedController);
	MainCharacterRef->GetCharacterMovement()->StopMovementImmediately();
	
}

