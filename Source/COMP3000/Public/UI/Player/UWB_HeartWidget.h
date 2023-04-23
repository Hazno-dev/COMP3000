// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystemWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "UWB_HeartWidget.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UUWB_HeartWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HPLBar Data")
	UTexture2D* HeartTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HPLBar Data")
	UTexture2D* NoHeartTexture;

	UFUNCTION()
	void SetHeartHealthy(bool bIsHealthy) const;

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UImage> HeartImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UNiagaraSystemWidget> HeartParticle;
	
};
