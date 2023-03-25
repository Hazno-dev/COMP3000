// Callum Stables, 2023


#include "Heroes/Abilities/AbilityBase.h"

#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAbilityBase::AAbilityBase() {
	PrimaryActorTick.bCanEverTick = true;

	// Set default values
	bIsAbilityPlaying = false;
	aSyncRecharge = false;
	CurrentCharges = MaxCharges;
	
	
	
}

void AAbilityBase::CustomBeginPlay(AMainCharacter* MainCharacter, UWorld* World) {
	Super::BeginPlay();
	
	MainCharacterRef = MainCharacter;
	CurrentCharges = MaxCharges;
	WorldRef = World;
	BaseCharacterSpeed = MainCharacterRef->GetCharacterMovement()->MaxWalkSpeed;
	BaseCharacterAcceleration = MainCharacterRef->GetCharacterMovement()->MaxAcceleration;

	CooldownTimerHandle.Invalidate();
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DashTry"));
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(CurrentCharges));
	
	if (!bIsAbilityPlaying && CurrentCharges > 0) {
		
		bIsAbilityPlaying = true;
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

void AAbilityBase::CoreEndAbility() {
	bIsAbilityPlaying = false;
	
	WorldRef->GetTimerManager().ClearTimer(AbilityEndTimerHandle);
	AbilityEndTimerHandle.Invalidate();

	if (!aSyncRecharge && !CooldownTimerHandle.IsValid()) {
		CoreStartCooldown();
	}
	
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
	
	EndCooldown();
}

void AAbilityBase::StartCooldown() {
}

void AAbilityBase::EndCooldown() {
}
