// Callum Stables, 2023


#include "Heroes/Abilities/AB_Dash.h"

#include "NiagaraFunctionLibrary.h"
#include "MainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Heroes/HeroGenerator.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Heroes/CameraDynamicMotion.h"
#include "Kismet/GameplayStatics.h"

void AAB_Dash::BeginAbility() {
	Super::BeginAbility();
	
	if (MainCharacterRef == nullptr) return;

	MainCharacterRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	MainCharacterRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	
	if (IsValid(DashExplosiveVFX))  UNiagaraFunctionLibrary::SpawnSystemAttached(DashExplosiveVFX, MainCharacterRef->GetMesh(), 
"Pelvis", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
	MainCharacterRef->HeroGeneratorComponent->SetMeshesHidden(true);

	for (FDashBones Bone : DashBones) {
		if (!IsValid(DashTrailVFX)) break;
		UNiagaraComponent* CreatedDashLine = UNiagaraFunctionLibrary::SpawnSystemAttached(DashTrailVFX, MainCharacterRef->GetMesh(),
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

	if (DashSoundStart && WorldRef) UGameplayStatics::PlaySoundAtLocation(WorldRef, DashSoundStart, MainCharacterRef->GetActorLocation(), 0.5f, 1.f, 0.f, DashSoundAttenuation);

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
	if (IsValid(DashExplosiveVFX)) UNiagaraFunctionLibrary::SpawnSystemAttached(DashExplosiveVFX, MainCharacterRef->GetMesh(), 
"Pelvis", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

	MainCharacterRef->GetCharacterMovement()->MaxWalkSpeed = BaseCharacterSpeed;
	MainCharacterRef->GetCharacterMovement()->MaxWalkSpeedCrouched = BaseCharacterSpeed;
	MainCharacterRef->GetCharacterMovement()->MaxAcceleration = BaseCharacterAcceleration;

	EnableInput(MainCharacterRef->SavedController);
	MainCharacterRef->GetCharacterMovement()->StopMovementImmediately();
	
	MainCharacterRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	MainCharacterRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);

	if (DashSoundEnd && WorldRef) UGameplayStatics::PlaySoundAtLocation(WorldRef, DashSoundEnd, MainCharacterRef->GetActorLocation(), 0.4f, 3.f, 0.f, DashSoundAttenuation);
	
}

