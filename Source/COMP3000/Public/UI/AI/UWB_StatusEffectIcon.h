// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "UWB_StatusEffectIcon.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UUWB_StatusEffectIcon : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	void SetStatusEffectIcon(UTexture2D* InStatusEffectIcon);

	UFUNCTION()
	void SetupMaterialInstance(UMaterialInterface* StatusEffectBaseMaterial);

	UFUNCTION()
	bool IsMaterialSetup() const { return IsValid(StatusMaterialInstance); }
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UImage> StatusEffectImage;

private:
	UMaterialInstanceDynamic* StatusMaterialInstance;
};
