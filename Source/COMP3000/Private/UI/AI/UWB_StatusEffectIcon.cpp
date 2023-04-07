// Callum Stables, 2023


#include "UI/AI/UWB_StatusEffectIcon.h"



void UUWB_StatusEffectIcon::SetStatusEffectIcon(UTexture2D* InStatusEffectIcon) {
	if (!IsValid(StatusMaterialInstance)) return;
	StatusMaterialInstance->SetTextureParameterValue("InIcon", InStatusEffectIcon);
}

void UUWB_StatusEffectIcon::SetupMaterialInstance(UMaterialInterface* StatusEffectBaseMaterial) {
	StatusMaterialInstance = UMaterialInstanceDynamic::Create(StatusEffectBaseMaterial, this);
	StatusEffectImage->SetBrushFromMaterial(StatusMaterialInstance);
}
