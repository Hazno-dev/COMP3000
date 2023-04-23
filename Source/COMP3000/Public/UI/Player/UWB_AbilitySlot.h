// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Material")
	UMaterialInterface* AbilityMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Material")
	UTexture2D* EmptyAbilityTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Bindings")
	FName AbilityKeybindMapping;

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//Panel Border
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UOverlay> AbilityChargesOverlay; 

	//Text for the ability
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UTextBlock> AbilityChargesText;

	//Text for the ability
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UTextBlock> AbilityKeybindText;

	//Image for the ability
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UImage> AbilityImage;

	//Image for the ability
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget, EditInlineNew, EditFixedSize))
	TObjectPtr<UImage> AbiltiyGlowBackground;

private:
	class AMainCharacter* MainCharacterRef;

	class AAbilityBase* AbilityRef;

	UMaterialInstanceDynamic* AbilityMaterialInstance;

	void DynamicCallbackSetup();
	void SetKeybindText();
	void SetIconCharges();
	
	UFUNCTION()
	void UpdateIcon();

	UFUNCTION()
	void BeganCooldown();

	UFUNCTION()
	void EndedCooldown();

	UFUNCTION()
	void BeganUsing();

	UFUNCTION()
	void EndedUsing();

	void InitMaterial();

	UFUNCTION()
	void RefreshAbility();

	UFUNCTION()
	void RefreshAbilityCharges();

	UFUNCTION()
	void RemoveCallbacks();

	bool bIsCharging = false;

	float ChargeValue = 0.f;
	float TargetValue = 0.f;

	bool bPlayGlow = false;
	float GlowValue = 0.f;
};
