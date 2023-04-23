// Callum Stables, 2023


#include "AI/Abilities/EnemyStatusEffectSystem.h"

// Sets default values for this component's properties
UEnemyStatusEffectSystem::UEnemyStatusEffectSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UEnemyStatusEffectSystem::BeginPlay()
{
	Super::BeginPlay();

	if (BaseStatusEffects.Num() > 0)
	{
		for (TSubclassOf<UStatusEffectBase> StatusEffectData : BaseStatusEffects)
		{
			AddStatusEffect(StatusEffectData);
		}
	}
	
}

UStatusEffectBase* UEnemyStatusEffectSystem::AddStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffectData, float Duration, float Strength)
{
	// Create an instance of the status effect
	UStatusEffectBase* StatusEffectBase = NewObject<UStatusEffectBase>(this, StatusEffectData);
	if (!IsValid(StatusEffectBase)) return nullptr;

	// Set the duration and strength
	if (Duration != -1.f) StatusEffectBase->SetDuration(Duration);
	if (Strength != -1.f) StatusEffectBase->SetStrength(Strength);
	
	UStatusEffectBase* ReturnedEffect = IsStatusEffectAlreadyApplied(StatusEffectBase);

	if (IsValid(ReturnedEffect) && !ReturnedEffect->bIsPermanent) RevokeStatusEffect(IsStatusEffectAlreadyApplied(StatusEffectBase));
	
	// Add the status effect to the array
	if (!IsValid(ReturnedEffect)) CurrentStatusEffects.Add(StatusEffectBase);
		

	// Gengine added a status effect
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Status Effect Added")));

	if (!StatusEffectBase->bIsPermanent)
	{
        GetWorld()->GetTimerManager().SetTimer(StatusEffectBase->StatusEffectTimerHandle, [this, StatusEffectBase]()
        {
            RevokeStatusEffect(StatusEffectBase);
        }, StatusEffectBase->Duration, false);
	}
	EventNewStatusEffect.Broadcast();
	return StatusEffectBase;
}

void UEnemyStatusEffectSystem::RevokeStatusEffect(UStatusEffectBase* StatusEffectData) {
	if (!IsValid(StatusEffectData)) return;
	
	// Remove the status effect from the array, cleanse timer
	GetWorld()->GetTimerManager().ClearTimer(StatusEffectData->StatusEffectTimerHandle);
	StatusEffectData->StatusEffectTimerHandle.Invalidate();
	
	CurrentStatusEffects.Remove(StatusEffectData);
	EventRevertStatusEffect.Broadcast();
}

TArray<UStatusEffectBase*> UEnemyStatusEffectSystem::GetStatusEffectsOfType(EStatusEffect StatusEffectType) {
	if (!IsValid(this)) return TArray<UStatusEffectBase*>();
	TArray<UStatusEffectBase*> StatusEffectsOfType;

	if (CurrentStatusEffects.Num() == 0) return StatusEffectsOfType;
	for (UStatusEffectBase* StatusEffect : CurrentStatusEffects)
	{
		if (StatusEffect->StatusEffect == StatusEffectType)
		{
			StatusEffectsOfType.Add(StatusEffect);
		}
	}
	return StatusEffectsOfType;
}

UStatusEffectBase* UEnemyStatusEffectSystem::IsStatusEffectAlreadyApplied(const UStatusEffectBase* InStatusEffectType) {
	if (!IsValid(InStatusEffectType)) return nullptr;
	if (CurrentStatusEffects.Num() == 0) return nullptr;
	
	for (UStatusEffectBase* StatusEffect : CurrentStatusEffects)
	{
		if (StatusEffect->StatusEffect == InStatusEffectType->StatusEffect)
		{
			if (StatusEffect->StatusEffect == VisualOnly && StatusEffect->StatusEffectIcon == InStatusEffectType->StatusEffectIcon) return nullptr;
			return StatusEffect;
		}
	}
	return nullptr;
}

