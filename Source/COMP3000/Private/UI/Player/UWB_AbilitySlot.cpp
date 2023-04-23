// Callum Stables, 2023


#include "UI/Player/UWB_AbilitySlot.h"

#include "MainCharacter.h"
#include "Components/Border.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Overlay.h"
#include "Heroes/PlayerBaseAbilities.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

void UUWB_AbilitySlot::NativeConstruct() {
	Super::NativeConstruct();

	MainCharacterRef = Cast<AMainCharacter>(GetOwningPlayerPawn());
	if (!IsValid(MainCharacterRef)) return;
	
	MainCharacterRef->PlayerBaseAbilitiesComponent->RefreshAbilityIcons.AddDynamic(this, &UUWB_AbilitySlot::RefreshAbility);
	
	AbilityRef = MainCharacterRef->PlayerBaseAbilitiesComponent->AbilityFromEnum(AbilitySlotType);
	
	InitMaterial();
	UpdateIcon();
	DynamicCallbackSetup();
	SetKeybindText();
	SetIconCharges();

	AbiltiyGlowBackground->SetOpacity(0.f);
}

void UUWB_AbilitySlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (MainCharacterRef == nullptr) return;

	if (bIsCharging) {
		if (ChargeValue <= TargetValue) {
			ChargeValue += InDeltaTime;
			AbilityMaterialInstance->SetScalarParameterValue("Value",
				UKismetMathLibrary::NormalizeToRange(ChargeValue, 0, TargetValue));
		}
	}
	if (bPlayGlow) {
		GlowValue = UKismetMathLibrary::FInterpTo(GlowValue, 1.f, InDeltaTime, 15.f);
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(GlowValue, 1.f, 0.01f)) {
			GlowValue = 0.f;
			bPlayGlow = false;
		}
		AbiltiyGlowBackground->SetOpacity(GlowValue);
	}
}

void UUWB_AbilitySlot::InitMaterial() {
	if (AbilityMaterial == nullptr) return;
	AbilityMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), AbilityMaterial);
	AbilityMaterialInstance->SetScalarParameterValue("Value", 0);
	AbilityImage->SetBrushFromMaterial(AbilityMaterialInstance);
}

void UUWB_AbilitySlot::RefreshAbility() {
	RemoveCallbacks();
	AbilityRef = MainCharacterRef->PlayerBaseAbilitiesComponent->AbilityFromEnum(AbilitySlotType);
	InitMaterial();
	UpdateIcon();
	DynamicCallbackSetup();
	SetKeybindText();
	SetIconCharges();
}

void UUWB_AbilitySlot::RefreshAbilityCharges() {
	SetIconCharges();
}

void UUWB_AbilitySlot::RemoveCallbacks() {
	if (!IsValid(AbilityRef)) return;
	
	AbilityRef->OnAbilityCooldownStart.RemoveDynamic(this, &UUWB_AbilitySlot::BeganCooldown);
	AbilityRef->OnAbilityCooldownEnd.RemoveDynamic(this, &UUWB_AbilitySlot::EndedCooldown);
	AbilityRef->OnAbilityStart.RemoveDynamic(this, &UUWB_AbilitySlot::BeganUsing);
	AbilityRef->OnAbilityEnd.RemoveDynamic(this, &UUWB_AbilitySlot::EndedUsing);
	AbilityRef->OnAbilityRefreshCharges.RemoveDynamic(this, &UUWB_AbilitySlot::RefreshAbilityCharges);
}

void UUWB_AbilitySlot::DynamicCallbackSetup() {
	if (AbilityRef == nullptr) return;
	
	AbilityRef->OnAbilityCooldownStart.AddDynamic(this, &UUWB_AbilitySlot::BeganCooldown);
	AbilityRef->OnAbilityCooldownEnd.AddDynamic(this, &UUWB_AbilitySlot::EndedCooldown);
	AbilityRef->OnAbilityStart.AddDynamic(this, &UUWB_AbilitySlot::BeganUsing);
	AbilityRef->OnAbilityEnd.AddDynamic(this, &UUWB_AbilitySlot::EndedUsing);
	AbilityRef->OnAbilityRefreshCharges.AddDynamic(this, &UUWB_AbilitySlot::RefreshAbilityCharges);
}

void UUWB_AbilitySlot::SetKeybindText() {
	if (!IsValid(MainCharacterRef)) return;
	if (MainCharacterRef->Subsystem == nullptr || AbilityKeybindText == nullptr) return;
	
	TArray<FEnhancedActionKeyMapping> Mappings = MainCharacterRef->Subsystem->GetAllPlayerMappableActionKeyMappings();

	if (Mappings.Num() == 0) {
		if (!IsValid(MainCharacterRef->BaseInputMappingContext)) return;
		
		Mappings = MainCharacterRef->BaseInputMappingContext->GetMappings();
	}
	
	for (FEnhancedActionKeyMapping Mapping : Mappings) {
		if (Mapping.PlayerMappableOptions.Name == AbilityKeybindMapping) {
			AbilityKeybindText->SetText(FText::FromString(Mapping.Key.ToString()));
			return;
		}
	}
}

void UUWB_AbilitySlot::SetIconCharges() {
	if (!IsValid(MainCharacterRef)) return;

	if (!IsValid(AbilityRef)) {
		AbilityChargesOverlay->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	if (AbilityRef->MaxCharges > 1) {
		AbilityChargesOverlay->SetVisibility(ESlateVisibility::Visible);
		AbilityChargesText->SetText(FText::FromString(FString::FromInt(AbilityRef->CurrentCharges)));
	} else AbilityChargesOverlay->SetVisibility(ESlateVisibility::Hidden);
	
}

void UUWB_AbilitySlot::UpdateIcon() {
	if (!IsValid(MainCharacterRef)) return;
	if (!IsValid(AbilityRef)) return;
	if (IsValid(AbilityRef->AbilityIcon)) {
		AbilityMaterialInstance->SetTextureParameterValue("InIcon", AbilityRef->AbilityIcon);
	} else {
		UTexture2D* NullTexture = nullptr;
		if (IsValid(EmptyAbilityTexture)) AbilityMaterialInstance->SetTextureParameterValue("InIcon", EmptyAbilityTexture);
		else AbilityMaterialInstance->SetTextureParameterValue("InIcon", NullTexture);
	}
}

void UUWB_AbilitySlot::BeganCooldown() {
	if (!IsValid(MainCharacterRef)) return;
	if (!IsValid(AbilityRef)) return;
	
	ChargeValue = 0;
	bIsCharging = true;
	TargetValue = AbilityRef->AbilityRechargeDuration;
}

void UUWB_AbilitySlot::EndedCooldown() {
	if(IsValid(AbilityRef)) AbilityChargesText->SetText(FText::FromString(FString::FromInt(AbilityRef->CurrentCharges)));
	
	bIsCharging = false;
	AbilityMaterialInstance->SetScalarParameterValue("Value", 0);
	bPlayGlow = true;
		
}

void UUWB_AbilitySlot::BeganUsing() {
	if(IsValid(AbilityRef)) AbilityChargesText->SetText(FText::FromString(FString::FromInt(AbilityRef->CurrentCharges)));
	
	AbilityMaterialInstance->SetScalarParameterValue("IconStrength", 5);
}

void UUWB_AbilitySlot::EndedUsing() {
	AbilityMaterialInstance->SetScalarParameterValue("IconStrength", 1);
}


