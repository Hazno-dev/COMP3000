// Callum Stables, 2023


#include "Heroes/PlayerBaseAbilities.h"

#include "MainCharacter.h"
#include "Heroes/Abilities/AB_Dash.h"

// Sets default values for this component's properties
UPlayerBaseAbilities::UPlayerBaseAbilities()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerBaseAbilities::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() == nullptr) return;
	
	MainCharacterRef = Cast<AMainCharacter>(GetOwner());

	DashAbilityInstance = NewObject<AAbilityBase>(this, DashClass);
	DashAbilityInstance->CustomBeginPlay(MainCharacterRef, GetWorld());
	
}


// Called every frame
void UPlayerBaseAbilities::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(DashAbilityInstance)) DashAbilityInstance->CustomTick(DeltaTime);
	if (IsValid(Ability1Instance)) Ability1Instance->CustomTick(DeltaTime);
	if (IsValid(Ability2Instance)) Ability2Instance->CustomTick(DeltaTime);
	if (IsValid(Ability3Instance)) Ability3Instance->CustomTick(DeltaTime);
	if (IsValid(UltimateAbilityInstance)) UltimateAbilityInstance->CustomTick(DeltaTime);
	

	// ...
}

void UPlayerBaseAbilities::StartDash() {
	if (!IsValid(MainCharacterRef) || !IsValid(DashAbilityInstance)) return;
	if (IsAnyAbilityPlayingOrCasting()) return;

	DashAbilityInstance->CoreBeginAbility();
	
	
}

void UPlayerBaseAbilities::StartAbility1() {
	if (!IsValid(MainCharacterRef) || !IsValid(Ability1Instance)) return;
	if (IsAnyAbilityPlayingOrCasting()) return;
	
	Ability1Instance->CoreBeginAbility();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ability 1"));
}

void UPlayerBaseAbilities::StartAbility2() {
	if (IsAnyAbilityPlayingOrCasting()) return;
}

void UPlayerBaseAbilities::StartAbility3() {
	if (IsAnyAbilityPlayingOrCasting()) return;
}

void UPlayerBaseAbilities::StartUltimate() {
	if (IsAnyAbilityPlayingOrCasting()) return;
}

AAbilityBase* UPlayerBaseAbilities::AbilityFromEnum(const EAbilitySlotType AbilitySlotType) const {
	switch (AbilitySlotType) {
		case EAbilitySlotType::Dash:
			if (DashAbilityInstance == nullptr) return nullptr;
			return DashAbilityInstance;
		case EAbilitySlotType::Ability1:
			if (Ability1Instance == nullptr) return nullptr;
			return Ability1Instance;
		case EAbilitySlotType::Ability2:
			if (Ability2Instance == nullptr) return nullptr;
			return Ability2Instance;
		case EAbilitySlotType::Ability3:
			if (Ability3Instance == nullptr) return nullptr;
			return Ability3Instance;
		case EAbilitySlotType::Ultimate:
			if (UltimateAbilityInstance == nullptr) return nullptr;
			return UltimateAbilityInstance;
		default:
			return nullptr;
	}
}

void UPlayerBaseAbilities::SetDashMaxCharges(const int32 NewCharges) {
	if (DashAbilityInstance == nullptr) return;
	if (NewCharges == DashAbilityInstance->MaxCharges) return;
	
	int32 oldMax = DashAbilityInstance->MaxCharges;

	//Update Max Charges
	DashAbilityInstance->MaxCharges = NewCharges;
	
	if (NewCharges > oldMax) {
		// If the new max is more than the old max and the current charges are equal to the old max, reset the cooldown
		if (DashAbilityInstance->CurrentCharges == oldMax) DashAbilityInstance->CoreStartCooldown();
	} else {
		// If the new max is less than the old max, broadcast CD end and lower current charges
		if (NewCharges < DashAbilityInstance->CurrentCharges) {
			DashAbilityInstance->CurrentCharges = NewCharges;
			DashAbilityInstance->OnAbilityCooldownEnd.Broadcast();
		}
		if (DashAbilityInstance->CurrentCharges == NewCharges) DashAbilityInstance->OnAbilityCooldownEnd.Broadcast();
	}
	DashAbilityInstance->OnAbilityRefreshCharges.Broadcast();
}

void UPlayerBaseAbilities::SetDashDuration(const float NewDuration) {
	if (DashAbilityInstance == nullptr) return;
	if (NewDuration == DashAbilityInstance->AbilityDuration) return;

	DashAbilityInstance->AbilityDuration = NewDuration;
}

void UPlayerBaseAbilities::SetDashVFX(UNiagaraSystem* Explosion, UNiagaraSystem* Trail) {
	if (DashAbilityInstance == nullptr) return;
	if (AAB_Dash* DashAbilityRef = Cast<AAB_Dash>(DashAbilityInstance)) {
		DashAbilityRef->DashExplosiveVFX = Explosion;
		DashAbilityRef->DashTrailVFX = Trail;
	}
}

void UPlayerBaseAbilities::SetAbility1Instance(AAbilityBase* NewAbility) {
	if (Ability1Instance == NewAbility) return;

	Ability1Instance = NewAbility;
}

void UPlayerBaseAbilities::SetAbility2Instance(AAbilityBase* NewAbility) {
	if (Ability2Instance == NewAbility) return;

	Ability2Instance = NewAbility;
}

void UPlayerBaseAbilities::SetAbility3Instance(AAbilityBase* NewAbility) {
	if (Ability3Instance == NewAbility) return;

	Ability3Instance = NewAbility;
}

void UPlayerBaseAbilities::SetUltimateAbilityInstance(AAbilityBase* NewAbility) {
	if (UltimateAbilityInstance == NewAbility) return;

	UltimateAbilityInstance = NewAbility;
}

void UPlayerBaseAbilities::CastTriggerAnyAbility() {
	if (!IsValid(MainCharacterRef)) return;

	FVector TargetLocation, TargetNormal;
	TargetLocation = MainCharacterRef->LastHitLocationCursor;
	TargetNormal = MainCharacterRef->LastHitNormalCursor;

	if (IsValid(Ability1Instance)) if (Ability1Instance->bRangeCastAbility && Ability1Instance->bIsCasting) Ability1Instance->CoreEndCasting(TargetLocation, TargetNormal);
	else if (IsValid(Ability2Instance)) if (Ability2Instance->bRangeCastAbility && Ability2Instance->bIsCasting) Ability2Instance->CoreEndCasting(TargetLocation, TargetNormal);
	else if (IsValid(Ability3Instance)) if (Ability3Instance->bRangeCastAbility && Ability3Instance->bIsCasting) Ability3Instance->CoreEndCasting(TargetLocation, TargetNormal);
	else if (IsValid(UltimateAbilityInstance)) if (UltimateAbilityInstance->bRangeCastAbility && UltimateAbilityInstance->bIsCasting) UltimateAbilityInstance->CoreEndCasting(TargetLocation, TargetNormal);
}

void UPlayerBaseAbilities::CastCancelAnyAbility() {
	if (!IsValid(MainCharacterRef)) return;

	if (IsValid(Ability1Instance)) if (Ability1Instance->bRangeCastAbility && Ability1Instance->bIsCasting) Ability1Instance->CoreCancelCasting();
	else if (IsValid(Ability2Instance)) if (Ability2Instance->bRangeCastAbility && Ability2Instance->bIsCasting) Ability2Instance->CoreCancelCasting();
	else if (IsValid(Ability3Instance)) if (Ability3Instance->bRangeCastAbility && Ability3Instance->bIsCasting) Ability3Instance->CoreCancelCasting();
	else if (IsValid(UltimateAbilityInstance)) if (UltimateAbilityInstance->bRangeCastAbility && UltimateAbilityInstance->bIsCasting) UltimateAbilityInstance->CoreCancelCasting();
}

bool UPlayerBaseAbilities::IsAnyAbilityPlayingOrCasting(bool bIgnoreCastingPlaying) const {
	if (!IsValid(MainCharacterRef)) return false;

	if (IsValid(DashAbilityInstance)) if (DashAbilityInstance->bIsAbilityPlaying) return true;
	if (IsValid(Ability1Instance)) {
		if (Ability1Instance->bIsAbilityPlaying && Ability1Instance->bRangeCastAbility && bIgnoreCastingPlaying) return false;
		if (Ability1Instance->bIsCasting || Ability1Instance->bIsAbilityPlaying) return true;
	}
	if (IsValid(Ability2Instance)) {
		if (Ability2Instance->bIsAbilityPlaying && Ability2Instance->bRangeCastAbility && bIgnoreCastingPlaying) return false;
		if (Ability2Instance->bIsCasting || Ability2Instance->bIsAbilityPlaying) return true;
	}
	if (IsValid(Ability3Instance)) {
		if (Ability3Instance->bIsAbilityPlaying && Ability3Instance->bRangeCastAbility && bIgnoreCastingPlaying) return false;
		if (Ability3Instance->bIsCasting || Ability3Instance->bIsAbilityPlaying) return true;
	}
	if (IsValid(UltimateAbilityInstance)) {
		if (UltimateAbilityInstance->bIsAbilityPlaying && UltimateAbilityInstance->bRangeCastAbility && bIgnoreCastingPlaying) return false;
		if (UltimateAbilityInstance->bIsCasting || UltimateAbilityInstance->bIsAbilityPlaying) return true;
	}

	return false;
}

bool UPlayerBaseAbilities::IsAnyAbilityCasting() const {
	if (!IsValid(MainCharacterRef)) return false;

	if (IsValid(Ability1Instance)) if (Ability1Instance->bIsCasting) return true;
	if (IsValid(Ability2Instance)) if (Ability2Instance->bIsCasting) return true;
	if (IsValid(Ability3Instance)) if (Ability3Instance->bIsCasting) return true;
	if (IsValid(UltimateAbilityInstance)) if (UltimateAbilityInstance->bIsCasting) return true;

	return false;

	
}

AAbilityBase* UPlayerBaseAbilities::GetCurrentCastingAbility() const {
	if (!IsValid(MainCharacterRef)) return nullptr;

	if (IsValid(Ability1Instance)) if (Ability1Instance->bIsCasting) return Ability1Instance;
	if (IsValid(Ability2Instance)) if (Ability2Instance->bIsCasting) return Ability2Instance;
	if (IsValid(Ability3Instance)) if (Ability3Instance->bIsCasting) return Ability3Instance;
	if (IsValid(UltimateAbilityInstance)) if (UltimateAbilityInstance->bIsCasting) return UltimateAbilityInstance;

	return nullptr;
}

