// Callum Stables, 2023


#include "UI/Player/UWB_AbilitySlot.h"

#include "MainCharacter.h"
#include "Components/Border.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Overlay.h"
#include "Heroes/PlayerBaseAbilities.h"

void UUWB_AbilitySlot::NativeConstruct() {
	Super::NativeConstruct();

	MainCharacterRef = Cast<AMainCharacter>(GetOwningPlayerPawn());
	UpdateIcon();
	AbilityProgressBar->SetPercent(0);
}

void UUWB_AbilitySlot::UpdateIcon() {
	if (MainCharacterRef == nullptr) return;
	if (MainCharacterRef->PlayerBaseAbilitiesComponent->AbilityTextureFromEnum(AbilitySlotType) != nullptr) {
		AbilityImage->SetBrushFromTexture(MainCharacterRef->PlayerBaseAbilitiesComponent->AbilityTextureFromEnum(AbilitySlotType));
	} else {
		UTexture2D* NullTexture = nullptr;
		AbilityImage->SetBrushFromTexture(NullTexture);
	}
}
