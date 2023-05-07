// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UWB_HotswapSlot.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UUWB_HotswapSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hotswap Data")
	UMaterialInterface* HotswapIconMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hotswap Data")
	UWidgetAnimation* UnSelectedAnim;

	UFUNCTION()
	void SetIconImage(UTexture2D* IconTexture);

	UFUNCTION()
	void SetBackgroundColour(FColor BackgroundColour);

	UFUNCTION()
	void SetupMaterialInstance();

	UFUNCTION()
	void Selected();

	UFUNCTION()
	void UnSelected();

	UFUNCTION()
	void SetKeybindText(FText KeybindText);

	UPROPERTY()
	bool bIsSelected;
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UImage> HotswapIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UTextBlock> HotswapKeybindText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UImage> BackgroundImage;
	
	UPROPERTY( Transient, meta = ( BindWidgetAnim ) )
	UWidgetAnimation* SelectedAnim;



private:

	UMaterialInstanceDynamic* HotswapIconMaterialInstance;
};
