// Callum Stables, 2023


#include "AI/Abilities/EAbilityBase.h"

#include "AI/BaseAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEAbilityBase::AEAbilityBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set default values
	bIsAbilityPlaying = false;
	aSyncRecharge = false;
	CurrentCharges = MaxCharges;
	AbilityType = EAbilityType::Damage;
	
}

void AEAbilityBase::CustomBeginPlay(ABaseAICharacter* AICharacter, UWorld* World) {
	Super::BeginPlay();

	if (!IsValid(AICharacter) || !IsValid(World)) return;
	
	AICharacterRef = AICharacter;
	CurrentCharges = MaxCharges;
	WorldRef = World;
	BaseCharacterSpeed = AICharacterRef->GetCharacterMovement()->MaxWalkSpeed;
	BaseCharacterAcceleration = AICharacterRef->GetCharacterMovement()->MaxAcceleration;
	bIsAbilityPlaying = false;
	AICharacterRef->TookDamageEvent.AddDynamic(this, &AEAbilityBase::CoreInterruptAbility);

	CooldownTimerHandle.Invalidate();
}

void AEAbilityBase::CustomTick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bIsAbilityPlaying) {
		PlayingAbility();
	}
	
}

void AEAbilityBase::CoreBeginAbility() {
	if (!IsValid(AICharacterRef)) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Ability: %s"), *GetName()));

	//gengine bability playing and current charges
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("bIsAbilityPlaying: %s"), bIsAbilityPlaying ? TEXT("true") : TEXT("false")));
	
	if (!bIsAbilityPlaying && CurrentCharges > 0) {
		bIsAbilityPlaying = true;
		bIsInterrupted = false;
		CurrentCharges--;

		// Call EndAbility function after specified duration has elapsed
		WorldRef->GetTimerManager().SetTimer(AbilityEndTimerHandle, this, &AEAbilityBase::CoreEndAbility, AbilityDuration, false);

		if (aSyncRecharge && !CooldownTimerHandle.IsValid()) {
			CoreStartCooldown();
		}
		
		BeginAbility();
	}
}

void AEAbilityBase::CoreEndAbility() {
	bIsAbilityPlaying = false;
	
	WorldRef->GetTimerManager().ClearTimer(AbilityEndTimerHandle);
	AbilityEndTimerHandle.Invalidate();

	if (!aSyncRecharge && !CooldownTimerHandle.IsValid()) {
		CoreStartCooldown();
	}
	
	EndAbility();
}

void AEAbilityBase::BeginAbility() {
}

void AEAbilityBase::PlayingAbility() {
}

void AEAbilityBase::EndAbility() {
}

void AEAbilityBase::CoreStartCooldown() {
	FTimerDelegate TimerDelegate;
	
	TimerDelegate.BindUFunction(this, "CoreEndCooldown");
	WorldRef->GetTimerManager().SetTimer(CooldownTimerHandle, TimerDelegate, AbilityRechargeDuration, false);

	StartCooldown();
}

void AEAbilityBase::CoreEndCooldown() {

	//Wipe memory of the timer handle
	WorldRef->GetTimerManager().ClearTimer(CooldownTimerHandle);
	CooldownTimerHandle.Invalidate();
	
	//Increment Charges
	if (CurrentCharges < MaxCharges) {
		CurrentCharges++;
		if (CurrentCharges < MaxCharges) {
			CoreStartCooldown();
		}
	}
	
	EndCooldown();
}

void AEAbilityBase::StartCooldown() {
}

void AEAbilityBase::EndCooldown() {
}

void AEAbilityBase::CoreInterruptAbility() {
	if (bIsAbilityPlaying) {
		bIsAbilityPlaying = false;
		bIsInterrupted = true;
		WorldRef->GetTimerManager().ClearTimer(AbilityEndTimerHandle);
		AbilityEndTimerHandle.Invalidate();
		WorldRef->GetTimerManager().ClearTimer(CooldownTimerHandle);
		CooldownTimerHandle.Invalidate();
		CoreStartCooldown();
		InterruptAbility();
	}
}

void AEAbilityBase::InterruptAbility() {
}

