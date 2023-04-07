// Callum Stables, 2023


#include "UI/AI/UWB_BasicText.h"

void UUWB_BasicText::NativeConstruct() {
	Super::NativeConstruct();
}

void UUWB_BasicText::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	TextBlock->SetOpacity(Opacity);
	FLinearColor Color = TextBlock->GetShadowColorAndOpacity();
	Color.A = Opacity;
	TextBlock->SetShadowColorAndOpacity(Color);
	Opacity = FMath::FInterpTo(Opacity, 0.f, InDeltaTime, FadeSpeed);
}
