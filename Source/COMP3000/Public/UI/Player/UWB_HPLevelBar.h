// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "UWB_HeartWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UWB_HPLevelBar.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UUWB_HPLevelBar : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HPLBar Data")
	TSubclassOf<UUWB_HeartWidget> HeartWidgetClass;

	UFUNCTION()
	void HeroSwapped();

protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UHorizontalBox> HPBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UProgressBar> XPBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY( Transient, meta = ( BindWidgetAnim ) )
	UWidgetAnimation* XPGainedBump;

private:

	UFUNCTION()
	void ReevaluateHealthIcons();

	UFUNCTION()
	void ReevaluateXPBar();

	UFUNCTION()
	void ReevaluateXPBarAnimated();

	UFUNCTION()
	void ReevaluateLevelText();

	class AMainCharacter* MainCharacterRef;

	TArray<UUWB_HeartWidget*> CreatedHeartWidgets;

	
};
