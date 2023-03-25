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

	DashClass.GetDefaultObject()->CustomBeginPlay(MainCharacterRef, GetWorld());
	
}


// Called every frame
void UPlayerBaseAbilities::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DashClass.GetDefaultObject()->CustomTick(DeltaTime);
	
	

	// ...
}

void UPlayerBaseAbilities::StartDash() {
	if (MainCharacterRef == nullptr || DashClass == nullptr) return;

	DashClass.GetDefaultObject()->CoreBeginAbility();
	
	
}

void UPlayerBaseAbilities::HeldDash() {
	
}

void UPlayerBaseAbilities::EndDash() {
	if (MainCharacterRef == nullptr) return;


}

void UPlayerBaseAbilities::StartAbility1() {
}

void UPlayerBaseAbilities::StartAbility2() {
}

void UPlayerBaseAbilities::StartAbility3() {
}

void UPlayerBaseAbilities::StartUltimate() {
}

AAbilityBase* UPlayerBaseAbilities::AbilityFromEnum(const EAbilitySlotType AbilitySlotType) const {
	switch (AbilitySlotType) {
		case EAbilitySlotType::Dash:
			if (DashClass == nullptr) return nullptr;
			return DashClass.GetDefaultObject();
		case EAbilitySlotType::Ability1:
			if (Ability1Class == nullptr) return nullptr;
			return Cast<AAbilityBase>(Ability1Class);
		case EAbilitySlotType::Ability2:
			if (Ability2Class == nullptr) return nullptr;
			return Cast<AAbilityBase>(Ability2Class);
		case EAbilitySlotType::Ability3:
			if (Ability3Class == nullptr) return nullptr;
			return Cast<AAbilityBase>(Ability3Class);
		case EAbilitySlotType::Ultimate:
			if (UltimateClass == nullptr) return nullptr;
			return Cast<AAbilityBase>(UltimateClass);
		default:
			return nullptr;
	}
}

