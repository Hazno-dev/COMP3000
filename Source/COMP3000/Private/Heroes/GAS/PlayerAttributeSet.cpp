// Callum Stables, 2023


#include "Heroes/GAS/PlayerAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "MainCharacter.h"
#include "Net/UnrealNetwork.h"


UPlayerAttributeSet::UPlayerAttributeSet() {
	
}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerAttributeSet, Health);
	DOREPLIFETIME(UPlayerAttributeSet, MaxHealth);
	DOREPLIFETIME(UPlayerAttributeSet, MoveSpeed);
}

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute()) {
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context =  Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Target = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float DeltaValue {0.f};

	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive) {
		DeltaValue = Data.EvaluatedData.Magnitude;
	}
	AMainCharacter* TargetCharacter {nullptr};

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) {
		AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<AMainCharacter>(Data.Target.AbilityActorInfo->AvatarActor.Get());
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		if (TargetCharacter) {
			//TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
		}
	}
}

void UPlayerAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedProperty) {

	UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystemComponent) {
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
		AbilitySystemComponent->ApplyModToAttributeUnsafe(AffectedProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UPlayerAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Health, OldHealth);
}

void UPlayerAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MaxHealth, OldMaxHealth);
}

void UPlayerAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldSpeed) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MoveSpeed, OldSpeed);
}
