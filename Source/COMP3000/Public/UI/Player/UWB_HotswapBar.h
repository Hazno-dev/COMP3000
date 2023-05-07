// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "UWB_HotswapSlot.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "UWB_HotswapBar.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UUWB_HotswapBar : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotswapBar Data")
	TSubclassOf<UUWB_HotswapSlot> HotswapSlotClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotswapBar Data")
	TArray<FColor> HotswapSlotColours;

protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UHorizontalBox> HotswapBox;

	UFUNCTION()
	void IconsSetup();
	
	UFUNCTION()
	void HeroSwapped();

private:
	class AMainCharacter* MainCharacterRef;

	
};
