// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/AI/UWB_StatusEffectIcon.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "UWB_EnemyStatusBar.generated.h"

/**
 * 
 */
class ABaseAICharacter;
UCLASS()
class COMP3000_API UUWB_EnemyStatusBar : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusBar Data")
	UMaterialInterface* StatusEffectBaseMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusBar Data")
	TSubclassOf<UUWB_StatusEffectIcon> StatusEffectIconClass;

	UFUNCTION()
	void SetupComponents(ABaseAICharacter* InAICharacter, float InHealthStatusBarDuration);

	UFUNCTION()
	void UpdateHealthBar();

	UFUNCTION()
	void SetHealthBarOpacity(float Opacity);

	UFUNCTION()
	void HealthBarCountdown();
	
protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UHorizontalBox> StatusEffectBox;

	UFUNCTION()
	void ReevaluateStatusEffects();

private:
	ABaseAICharacter* AICharacterRef;

	float HealthStatusBarDuration;

	FTimerHandle HealthBarTimerHandle;
};
