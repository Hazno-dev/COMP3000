// Callum Stables, 2023


#include "UI/AI/UWB_EnemyStatusBar.h"
#include "Heroes/Abilities/StatusEffectBase.h"

#include "AI/BaseAICharacter.h"


void UUWB_EnemyStatusBar::SetupComponents(ABaseAICharacter* InAICharacter, float InHealthStatusBarDuration) {
	AICharacterRef = InAICharacter;
	HealthStatusBarDuration = InHealthStatusBarDuration;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(HealthStatusBarDuration));
	
	if (!IsValid(AICharacterRef)) return;

	HealthBar->SetPercent(1);
	
	AICharacterRef->EnemyStatusEffectSystemComponent->EventNewStatusEffect.AddDynamic(this, &UUWB_EnemyStatusBar::ReevaluateStatusEffects);
	AICharacterRef->EnemyStatusEffectSystemComponent->EventRevertStatusEffect.AddDynamic(this, &UUWB_EnemyStatusBar::ReevaluateStatusEffects);
	AICharacterRef->TookDamageEvent.AddDynamic(this, &UUWB_EnemyStatusBar::UpdateHealthBar);
	AICharacterRef->HealthChangedEvent.AddDynamic(this, &UUWB_EnemyStatusBar::UpdateHealthBar);
	AICharacterRef->DetectedEvent.AddDynamic(this, &UUWB_EnemyStatusBar::UpdateHealthBar);
	
	ReevaluateStatusEffects();
}

void UUWB_EnemyStatusBar::UpdateHealthBar() {
	if (!IsValid(AICharacterRef)) return;

	HealthBar->SetPercent(AICharacterRef->GetHealth() / AICharacterRef->GetMaxHealth());
	
	HealthBarCountdown();
}

void UUWB_EnemyStatusBar::SetHealthBarOpacity(float Opacity) {
	if (!IsValid(HealthBar)) return;
	HealthBar->SetRenderOpacity(Opacity);
}

void UUWB_EnemyStatusBar::HealthBarCountdown() {
	if (!IsValid(AICharacterRef)) return;

	SetHealthBarOpacity(true);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HealthBarCountdown")));

	if (IsValid(GetWorld())) {

		GetWorld()->GetTimerManager().ClearTimer(HealthBarTimerHandle);
		HealthBarTimerHandle.Invalidate();

		// Create a timer delegate and bind it to the SetHealthBarVisible function
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("SetHealthBarOpacity"), 0.4f);

		// Set a new timer to call SetHealthBarVisible with a value of false after the duration passes
		GetWorld()->GetTimerManager().SetTimer(HealthBarTimerHandle, TimerDelegate, HealthStatusBarDuration, false);
	}

}


void UUWB_EnemyStatusBar::NativeConstruct() {
	Super::NativeConstruct();
}

void UUWB_EnemyStatusBar::ReevaluateStatusEffects() {
	if (!IsValid(AICharacterRef)) return;

	StatusEffectBox->ClearChildren();

	TArray<UStatusEffectBase*> StatusEffects = AICharacterRef->EnemyStatusEffectSystemComponent->GetCurrentStatusEffects();

	if (StatusEffects.Num() == 0) return;

	for (auto StatusEffect : StatusEffects) {
		UUWB_StatusEffectIcon* StatusEffectIcon = CreateWidget<UUWB_StatusEffectIcon>(GetWorld(), StatusEffectIconClass);
		if (!IsValid(StatusEffectIcon)) continue;

		if(IsValid(StatusEffectBaseMaterial) && !StatusEffectIcon->IsMaterialSetup())
			StatusEffectIcon->SetupMaterialInstance(StatusEffectBaseMaterial);
		if(IsValid(StatusEffect->StatusEffectIcon)) StatusEffectIcon->SetStatusEffectIcon(StatusEffect->StatusEffectIcon);

		StatusEffectBox->AddChildToHorizontalBox(StatusEffectIcon);
	}
}

