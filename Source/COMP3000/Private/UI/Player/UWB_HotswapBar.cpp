// Callum Stables, 2023


#include "UI/Player/UWB_HotswapBar.h"

#include "MainCharacter.h"
#include "Kismet/KismetMaterialLibrary.h"

void UUWB_HotswapBar::NativeConstruct() {
	Super::NativeConstruct();
	
	
	MainCharacterRef = Cast<AMainCharacter>(GetOwningPlayerPawn());
	if (!IsValid(MainCharacterRef)) return;

	if (!IsValid(MainCharacterRef->HeroManagerComponent)) return;
	
	if (!MainCharacterRef->HeroManagerComponent->GetIconsLoaded()) MainCharacterRef->IconsSetup.AddDynamic(this, &UUWB_HotswapBar::IconsSetup);
	else IconsSetup();
	
}

//Setup Icons for hotswap bar
void UUWB_HotswapBar::IconsSetup() {
	if (!IsValid(HotswapSlotClass)) return;
	
	MainCharacterRef->HeroManagerComponent->HeroSwappedEvent.AddDynamic(this, &UUWB_HotswapBar::HeroSwapped);

	for (int i = 0; i < 4; i++) {
		UUWB_HotswapSlot* CreatedWidget = CreateWidget<UUWB_HotswapSlot>(GetWorld(), HotswapSlotClass);
		
		if (!IsValid(CreatedWidget)) continue;
		if (!IsValid(MainCharacterRef->HeroManagerComponent->GetHeroIconByIndex(i))) continue;

		CreatedWidget->SetupMaterialInstance();

		if (HotswapSlotColours.IsValidIndex(i)) CreatedWidget->SetBackgroundColour(HotswapSlotColours[i]);
		
		CreatedWidget->SetIconImage(MainCharacterRef->HeroManagerComponent->GetHeroIconByIndex(i));
		if (MainCharacterRef->HeroManagerComponent->CurrentHeroIndex == i) CreatedWidget->Selected();

		CreatedWidget->SetKeybindText(FText::FromString(FString::FromInt(i + 1)));
		
		HotswapBox->AddChild(CreatedWidget);
	}
}

void UUWB_HotswapBar::HeroSwapped() {
	if (!IsValid(MainCharacterRef)) return;
	if (!IsValid(MainCharacterRef->HeroManagerComponent)) return;

	for (int i = 0; i < HotswapBox->GetChildrenCount(); i++) {
		UUWB_HotswapSlot* CreatedWidget = Cast<UUWB_HotswapSlot>(HotswapBox->GetChildAt(i));
		if (!IsValid(CreatedWidget)) continue;
		
		if (MainCharacterRef->HeroManagerComponent->CurrentHeroIndex == i) CreatedWidget->Selected();
		else if (CreatedWidget->bIsSelected) CreatedWidget->UnSelected();
	}
	
}
