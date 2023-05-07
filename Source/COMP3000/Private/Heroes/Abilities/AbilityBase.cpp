// Callum Stables, 2023


#include "Heroes/Abilities/AbilityBase.h"

#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAbilityBase::AAbilityBase() {
	PrimaryActorTick.bCanEverTick = false;

	// Set default values
	bIsAbilityPlaying = false;
	aSyncRecharge = false;
	CurrentCharges = MaxCharges;
	bRangeCastAbility = false;
	bIsCasting = false;
	RangeCastSize = FVector(1.f);
	bIsWorldCursorVisible = true;
	Damage = 0.f;
	
}

void AAbilityBase::CustomBeginPlay(AMainCharacter* MainCharacter, UWorld* World) {
	Super::BeginPlay();
	
	MainCharacterRef = MainCharacter;
	CurrentCharges = MaxCharges;
	WorldRef = World;
	BaseCharacterSpeed = MainCharacterRef->GetCharacterMovement()->MaxWalkSpeed;
	BaseCharacterAcceleration = MainCharacterRef->GetCharacterMovement()->MaxAcceleration;

	this->SetOwner(MainCharacterRef);

	CooldownTimerHandle.Invalidate();
}

void AAbilityBase::CustomTick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bIsAbilityPlaying) {
		PlayingAbility();
	}
	
}

void AAbilityBase::CoreBeginAbility() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DashTry"));
	//current charges gengine
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(CurrentCharges));

	if (!IsValid(MainCharacterRef)) return;

	if (bRangeCastAbility && bIsCasting) return;
	
	if (!bIsAbilityPlaying && CurrentCharges > 0) {
		
		bIsAbilityPlaying = true;

		if (bRangeCastAbility) CoreStartCasting();
		else {
			CurrentCharges--;

			// Call EndAbility function after specified duration has elapsed
			WorldRef->GetTimerManager().SetTimer(AbilityEndTimerHandle, this, &AAbilityBase::CoreEndAbility, AbilityDuration, false);

			if (aSyncRecharge && !CooldownTimerHandle.IsValid()) {
				CoreStartCooldown();
			}
		
			BeginAbility();
			OnAbilityStart.Broadcast();
		}
	}
}

void AAbilityBase::CoreEndAbility() {
	bIsAbilityPlaying = false;
	
	WorldRef->GetTimerManager().ClearTimer(AbilityEndTimerHandle);
	AbilityEndTimerHandle.Invalidate();

	if (!aSyncRecharge && !CooldownTimerHandle.IsValid()) {
		CoreStartCooldown();
	}

	if (IsValid(SpawnedWorldCursorRef)) SpawnedWorldCursorRef->Destroy();
	
	EndAbility();
	OnAbilityEnd.Broadcast();
}

void AAbilityBase::BeginAbility() {
}

void AAbilityBase::PlayingAbility() {
}

void AAbilityBase::EndAbility() {
}

void AAbilityBase::CoreStartCooldown() {
	OnAbilityCooldownStart.Broadcast();
	FTimerDelegate TimerDelegate;
	
	TimerDelegate.BindUFunction(this, "CoreEndCooldown");
	WorldRef->GetTimerManager().SetTimer(CooldownTimerHandle, TimerDelegate, AbilityRechargeDuration, false);

	StartCooldown();
}

void AAbilityBase::CoreEndCooldown() {

	//Wipe memory of the timer handle
	WorldRef->GetTimerManager().ClearTimer(CooldownTimerHandle);
	CooldownTimerHandle.Invalidate();
	
	//Increment Charges
	if (CurrentCharges < MaxCharges) {
		CurrentCharges++;
		OnAbilityCooldownEnd.Broadcast();
		if (CurrentCharges < MaxCharges) {
			CoreStartCooldown();
		}
	}
	else OnAbilityCooldownEnd.Broadcast();
	
	EndCooldown();
}

void AAbilityBase::StartCooldown() {
}

void AAbilityBase::EndCooldown() {
}

void AAbilityBase::CoreStartCasting() {
	MainCharacterRef->CastingStart.Broadcast();

	bIsCasting = true;
	
	if (IsValid(MainCharacterRef->Subsystem) && IsValid(MainCharacterRef->AOEInputMappingContext)) {
		if (!MainCharacterRef->Subsystem->HasMappingContext(MainCharacterRef->AOEInputMappingContext)) 
			MainCharacterRef->Subsystem->AddMappingContext(MainCharacterRef->AOEInputMappingContext, MainCharacterRef->BaseMappingPriority + 1);
	}
	
	MainCharacterRef->Punching = false;
	MainCharacterRef->WorldCursor->ToggleVisibility(true);
}

void AAbilityBase::CoreEndCasting(FVector TargetLocation, FVector TargetNormal) {
	MainCharacterRef->CastingFinish.Broadcast();

	CastLocation = TargetLocation;
	CastNormal = TargetNormal;

	if (IsValid(MainCharacterRef->Subsystem) && IsValid(MainCharacterRef->AOEInputMappingContext)) {
		if (MainCharacterRef->Subsystem->HasMappingContext(MainCharacterRef->AOEInputMappingContext)) 
			MainCharacterRef->Subsystem->RemoveMappingContext(MainCharacterRef->AOEInputMappingContext);
	}
	
	CurrentCharges--;

	// Call EndAbility function after specified duration has elapsed
	WorldRef->GetTimerManager().SetTimer(AbilityEndTimerHandle, this, &AAbilityBase::CoreEndAbility, AbilityDuration, false);

	if (aSyncRecharge && !CooldownTimerHandle.IsValid()) {
		CoreStartCooldown();
	}
		
	BeginAbility();
	OnAbilityStart.Broadcast();

	if (bIsWorldCursorVisible) {
		if (IsValid(WorldCursorBP)) SpawnedWorldCursorRef = WorldRef->SpawnActor<AWorldCursor>(WorldCursorBP, TargetLocation, FRotator::ZeroRotator);
		SpawnedWorldCursorRef->ScaleCursor(RangeCastSize);
		SpawnedWorldCursorRef->MoveCursor(TargetLocation, TargetNormal);
	}

	MainCharacterRef->WorldCursor->ToggleVisibility(false);
	bIsCasting = false;
}

void AAbilityBase::CoreCancelCasting() {
	MainCharacterRef->CastingCancel.Broadcast();

	if (IsValid(MainCharacterRef->Subsystem) && IsValid(MainCharacterRef->AOEInputMappingContext)) {
		if (MainCharacterRef->Subsystem->HasMappingContext(MainCharacterRef->AOEInputMappingContext)) 
			MainCharacterRef->Subsystem->RemoveMappingContext(MainCharacterRef->AOEInputMappingContext);
	}

	MainCharacterRef->WorldCursor->ToggleVisibility(false);
	bIsCasting = false;
	bIsAbilityPlaying = false;
}

