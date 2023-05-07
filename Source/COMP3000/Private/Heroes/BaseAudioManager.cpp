// Callum Stables, 2023


#include "Heroes/BaseAudioManager.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBaseAudioManager::UBaseAudioManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


}

// Called when the game starts
void UBaseAudioManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBaseAudioManager::PlayHurtSound(bool bIs2D) {
	if (!HurtSound) return;
	if (!IsValid(GetWorld())) return;

	if (bIs2D) UGameplayStatics::PlaySound2D(GetWorld(), HurtSound);
	else {
		if (HurtAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), HurtSound, GetComponentLocation(), 1, 1, 0, HurtAttenuation);
		else if (BaseAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), HurtSound, GetComponentLocation(), 1, 1, 0, BaseAttenuation);
		else UGameplayStatics::PlaySoundAtLocation(GetWorld(), HurtSound, GetComponentLocation());
	}
}

void UBaseAudioManager::PlayDeathSound(bool bIs2D) {
	if (!DeathSound) return;
	if (!IsValid(GetWorld())) return;

	if (bIs2D) UGameplayStatics::PlaySound2D(GetWorld(), DeathSound);
	else {
		if (DeathAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetComponentLocation(), 1, 1, 0, DeathAttenuation);
		else if (BaseAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetComponentLocation(), 1, 1, 0, BaseAttenuation);
		else UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetComponentLocation());
	}
}

void UBaseAudioManager::PlayBaseAttackSound(bool bIs2D) {
	if (!BaseAttackSound) return;
	if (!IsValid(GetWorld())) return;

	if (bIs2D) UGameplayStatics::PlaySound2D(GetWorld(), BaseAttackSound);
	else {
		if (BaseAttackAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), BaseAttackSound, GetComponentLocation(), 1, 1, 0, BaseAttackAttenuation);
		else if (BaseAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), BaseAttackSound, GetComponentLocation(), 1, 1, 0, BaseAttenuation);
		else UGameplayStatics::PlaySoundAtLocation(GetWorld(), BaseAttackSound, GetComponentLocation());
	}
}

void UBaseAudioManager::PlayFootstepSound(bool bIs2D) {
	if (FootstepSounds.IsEmpty()) return;
	if (!IsValid(GetWorld())) return;

	int32 RandomIndex = FMath::RandRange(0, FootstepSounds.Num() - 1);
	
	if (!FootstepSounds[RandomIndex]) return;

	if (PreviousFootstepIndex == RandomIndex) {
		if (RandomIndex > 0) RandomIndex--;
		else RandomIndex++;
	}

	PreviousFootstepIndex = RandomIndex;

	if (bIs2D) UGameplayStatics::PlaySound2D(GetWorld(), FootstepSounds[RandomIndex], 0.1f);
	else {
		if (FootstepAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepSounds[RandomIndex], GetComponentLocation(), 0.1f, 1, 0, FootstepAttenuation);
		else if (BaseAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepSounds[RandomIndex], GetComponentLocation(), 0.1f, 1, 0, BaseAttenuation);
		else UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepSounds[RandomIndex], GetComponentLocation(), 0.1f);
	}
}

void UBaseAudioManager::PlayProjectileSummonSound(bool bIs2D) {
	if (!ProjectileSummonSound) return;
	if (!IsValid(GetWorld())) return;

	if (bIs2D) UGameplayStatics::PlaySound2D(GetWorld(), ProjectileSummonSound, 0.2f);
	else {
		if (ProjectileSummonAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSummonSound, GetComponentLocation(), 0.2f, 1, 0, ProjectileSummonAttenuation);
		else if (BaseAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSummonSound, GetComponentLocation(), 0.2f, 1, 0, BaseAttenuation);
		else UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSummonSound, GetComponentLocation(), 0.2f);
	}
}

void UBaseAudioManager::PlayAlertedSound(bool bIs2D) {
	if (!AlertedSound) return;
	if (!IsValid(GetWorld())) return;

	if (bIs2D) UGameplayStatics::PlaySound2D(GetWorld(), AlertedSound, .4f);
	else {
		if (AlertedAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), AlertedSound, GetComponentLocation(), .4f, 1, 0, AlertedAttenuation);
		else if (BaseAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), AlertedSound, GetComponentLocation(), .4f, 1, 0, BaseAttenuation);
		else UGameplayStatics::PlaySoundAtLocation(GetWorld(), AlertedSound, GetComponentLocation(), .4f);
	}
}

void UBaseAudioManager::PlayScreamSound(bool bIs2D) {
	if (!ScreamSound) return;
	if (!IsValid(GetWorld())) return;

	if (bIs2D) UGameplayStatics::PlaySound2D(GetWorld(), ScreamSound, .4f);
	else {
		if (ScreamAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ScreamSound, GetComponentLocation(), .4f, 1, 0, ScreamAttenuation);
		else if (BaseAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ScreamSound, GetComponentLocation(), .4f, 1, 0, BaseAttenuation);
		else UGameplayStatics::PlaySoundAtLocation(GetWorld(), ScreamSound, GetComponentLocation(), .4f);
	}
}

void UBaseAudioManager::PlayLevelUpSound() {
	if (!LevelUpSound) return;
	if (!IsValid(GetWorld())) return;

	UGameplayStatics::PlaySound2D(GetWorld(), LevelUpSound, 0.5f);
}

void UBaseAudioManager::PlayJumpSound() {
	if (!JumpSound) return;
	if (!IsValid(GetWorld())) return;

	UGameplayStatics::PlaySound2D(GetWorld(), JumpSound, 0.03f);
}

void UBaseAudioManager::PlayHeroSwapSound() {
	if (!HeroSwapSound) return;
	if (!IsValid(GetWorld())) return;

	UGameplayStatics::PlaySound2D(GetWorld(), HeroSwapSound, 0.7f);
}


