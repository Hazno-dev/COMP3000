// Callum Stables, 2023


#include "UI/Player/UWB_HPLevelBar.h"

#include "MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UUWB_HPLevelBar::NativeConstruct() {
	Super::NativeConstruct();

	MainCharacterRef = Cast<AMainCharacter>(GetOwningPlayerPawn());
	if (!IsValid(MainCharacterRef)) return;

	if (!IsValid(MainCharacterRef->HeroManagerComponent)) return;
	
	MainCharacterRef->HeroManagerComponent->HeroSwappedEvent.AddDynamic(this, &UUWB_HPLevelBar::HeroSwapped);
	MainCharacterRef->HeroManagerComponent->LevelUpEvent.AddDynamic(this, &UUWB_HPLevelBar::ReevaluateLevelText);
	MainCharacterRef->HeroManagerComponent->LevelUpEvent.AddDynamic(this, &UUWB_HPLevelBar::ReevaluateXPBar);
	MainCharacterRef->HeroManagerComponent->XPChangedEvent.AddDynamic(this, &UUWB_HPLevelBar::ReevaluateXPBarAnimated);
	MainCharacterRef->HeroManagerComponent->HPChangedEvent.AddDynamic(this, &UUWB_HPLevelBar::ReevaluateHealthIcons);

	HPBox->ClearChildren();
	
	// TODO: Add event for when the hero's health changes

	HeroSwapped();
}

void UUWB_HPLevelBar::HeroSwapped() {

	
	ReevaluateHealthIcons();
	ReevaluateXPBar();
	ReevaluateLevelText();
}

void UUWB_HPLevelBar::ReevaluateHealthIcons() {
	if (!IsValid(HeartWidgetClass)) return;

	const int CurrentHealth = MainCharacterRef->HeroManagerComponent->GetCurrentHP();
	const int MaxHealth = MainCharacterRef->HeroManagerComponent->GetCurrentHPMax();
	
	// Create any new hearts that are needed and/or swap health texture
	for (int i = 0; i < MaxHealth; i++) {
		if (!CreatedHeartWidgets.IsValidIndex(i)) {
			CreatedHeartWidgets.Add( CreateWidget<UUWB_HeartWidget>(GetWorld(), HeartWidgetClass));
			HPBox->AddChild(CreatedHeartWidgets[i]);
		}
		if (!CreatedHeartWidgets.IsValidIndex(i)) continue;
		
		
		if (i <= CurrentHealth - 1) CreatedHeartWidgets[i]->SetHeartHealthy(true);
		if (i > CurrentHealth - 1) CreatedHeartWidgets[i]->SetHeartHealthy(false);
	}

	// Remove any extra hearts that are not needed
	if (CreatedHeartWidgets.Num() > MaxHealth) {
		for (int i = MaxHealth; i < CreatedHeartWidgets.Num(); i++) {
			CreatedHeartWidgets[i]->RemoveFromParent();
		}
		CreatedHeartWidgets.RemoveAt(MaxHealth, CreatedHeartWidgets.Num() - MaxHealth);
	}
}

void UUWB_HPLevelBar::ReevaluateXPBar() {

	const float Min = MainCharacterRef->HeroManagerComponent->GetCurrentXPFloor();
	const float Max = MainCharacterRef->HeroManagerComponent->GetCurrentXPMax();
	const float Current = MainCharacterRef->HeroManagerComponent->GetCurrentXP();

	const float Percent = UKismetMathLibrary::NormalizeToRange(Current, Min, Max);
	
	XPBar->SetPercent(Percent);
	
}

void UUWB_HPLevelBar::ReevaluateXPBarAnimated() {
	
	const float Min = MainCharacterRef->HeroManagerComponent->GetCurrentXPFloor();
	const float Max = MainCharacterRef->HeroManagerComponent->GetCurrentXPMax();
	const float Current = MainCharacterRef->HeroManagerComponent->GetCurrentXP();

	const float Percent = UKismetMathLibrary::NormalizeToRange(Current, Min, Max);
	
	XPBar->SetPercent(Percent);
	
	PlayAnimation(XPGainedBump);
}

void UUWB_HPLevelBar::ReevaluateLevelText() {
	LevelText->SetText(FText::FromString(FString::FromInt(MainCharacterRef->HeroManagerComponent->GetCurrentLevel())));
}
