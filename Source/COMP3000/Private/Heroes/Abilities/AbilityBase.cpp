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

		if (aSyncRecharge) {
			CoreStartCooldown();
		}
		
		BeginAbility();
	}
}

void AAbilityBase::CoreEndAbility() {
	bIsAbilityPlaying = false;
	
	WorldRef->GetTimerManager().ClearTimer(AbilityEndTimerHandle);
	AbilityEndTimerHandle.Invalidate();

	if (!aSyncRecharge) {
		CoreStartCooldown();
	}
	
	EndAbility();
}

void AAbilityBase::BeginAbility() {
}

void AAbilityBase::PlayingAbility() {
}

void AAbilityBase::EndAbility() {
}

void AAbilityBase::CoreStartCooldown() {	
	FTimerDelegate TimerDelegate;
	CurrentChargeIndex = (CurrentChargeIndex + 1) % 1000;
	
	TimerDelegate.BindUFunction(this, "CoreEndCooldown", CurrentChargeIndex);
	CooldownTimerHandles.Add(CurrentChargeIndex, FTimerHandle());
	WorldRef->GetTimerManager().SetTimer(*CooldownTimerHandles.Find(CurrentChargeIndex), TimerDelegate, AbilityRechargeDuration, false);

	StartCooldown();
}

void AAbilityBase::CoreEndCooldown(int ChargeIndex) {

	//Wipe memory of the timer handle
	WorldRef->GetTimerManager().ClearTimer(*CooldownTimerHandles.Find(ChargeIndex));
	CooldownTimerHandles.Find(ChargeIndex)->Invalidate();
	CooldownTimerHandles.Remove(ChargeIndex);
	
	//Increment Charges
	if (CurrentCharges < MaxCharges) {
		CurrentCharges++;
	}
	EndCooldown();
}

void AAbilityBase::StartCooldown() {
}

void AAbilityBase::EndCooldown() {
}
