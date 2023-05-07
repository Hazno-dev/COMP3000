// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UWB_BasicText.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UUWB_BasicText : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	float FadeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	bool bAutoFade = true;

protected:
	
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UTextBlock> TextBlock;

private:
	float Opacity = 2.0f;
	
};
