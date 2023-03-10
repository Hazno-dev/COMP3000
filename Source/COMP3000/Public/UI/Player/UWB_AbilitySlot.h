// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Heroes/Abilities/AbilityData.h"
#include "UWB_AbilitySlot.generated.h"


/**
 * 
 */
UCLASS()
class COMP3000_API UUWB_AbilitySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Slot Type")
	TEnumAsByte<EAbilitySlotType> AbilitySlotType;

protected:

	virtual void NativeConstruct() override;

	//Panel Border
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UPanelWidget> BorderWidget;

	//Overlay
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UOverlay> AbilityOverlay;

	//Image for the ability
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UImage> AbilityImage;

	//Progress Bar
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UProgressBar> AbilityProgressBar;

private:
	class AMainCharacter* MainCharacterRef;

	void UpdateIcon();
	
};
