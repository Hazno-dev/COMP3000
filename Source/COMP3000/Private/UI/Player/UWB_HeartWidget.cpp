// Callum Stables, 2023


#include "UI/Player/UWB_HeartWidget.h"

void UUWB_HeartWidget::NativeConstruct() {
	Super::NativeConstruct();

	HeartImage->SetBrushFromTexture(HeartTexture);
}

void UUWB_HeartWidget::SetHeartHealthy(bool bIsHealthy) const {
	if (bIsHealthy) {
		HeartImage->SetBrushFromTexture(HeartTexture);
		HeartParticle->ActivateSystem(false);
	}
	else {
		HeartImage->SetBrushFromTexture(NoHeartTexture);
		HeartParticle->DeactivateSystem();
	}
}
