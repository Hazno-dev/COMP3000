// Callum Stables, 2023


#include "UI/Player/UWB_HotswapSlot.h"

#include "Kismet/KismetMaterialLibrary.h"

void UUWB_HotswapSlot::NativeConstruct() {
	Super::NativeConstruct();
}

void UUWB_HotswapSlot::SetIconImage(UTexture2D* IconTexture) {
	if (!IsValid(HotswapIconMaterialInstance)) return;
	
	HotswapIconMaterialInstance->SetTextureParameterValue("TexIn", IconTexture);
	HotswapIcon->SetBrushFromMaterial(HotswapIconMaterialInstance);
}

void UUWB_HotswapSlot::SetBackgroundColour(FColor BackgroundColour) {
	BackgroundImage->SetBrushTintColor(BackgroundColour);
}

void UUWB_HotswapSlot::SetupMaterialInstance() {
	HotswapIconMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(
	GetWorld(), HotswapIconMaterial);

	bIsSelected = false;
}

void UUWB_HotswapSlot::Selected() {
	if (!IsValid(HotswapIconMaterialInstance)) return;

	PlayAnimation(SelectedAnim);
	bIsSelected = true;
}

void UUWB_HotswapSlot::UnSelected() {
	if (!IsValid(HotswapIconMaterialInstance)) return;

	PlayAnimationReverse(SelectedAnim);
	bIsSelected = false;
}

void UUWB_HotswapSlot::SetKeybindText(FText KeybindText) {
	HotswapKeybindText->SetText(KeybindText);
}
