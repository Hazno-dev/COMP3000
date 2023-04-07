// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Heroes/Abilities/AbilityData.h"
#include "Heroes/Abilities/StatusEffectBase.h"
#include "EnemyStatusEffectSystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNewStatusEffect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRevertStatusEffect);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMP3000_API UEnemyStatusEffectSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyStatusEffectSystem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effects")
	TArray<TSubclassOf<UStatusEffectBase>> BaseStatusEffects;
	
	//Getters and Setters

	//Add a status effect, with optional parameters for duration/strength (Mostly for visual-only status effects)
	UFUNCTION(BlueprintCallable)
	UStatusEffectBase* AddStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffectData, float Duration = -1.0f, float Strength = -1.0f);
	
	UFUNCTION()
	void RevokeStatusEffect(UStatusEffectBase* StatusEffectData);

	UFUNCTION(BlueprintCallable)
	TArray<UStatusEffectBase*> GetStatusEffectsOfType(EStatusEffect StatusEffectType);
	
	UFUNCTION(BlueprintCallable)
	TArray<UStatusEffectBase*> GetCurrentStatusEffects() const { return CurrentStatusEffects; }

	//Status Effect Influence Getters
	UFUNCTION(BlueprintCallable)
	float GetMovementSpeedInfluence() const { return MovementSpeedInfluence; }

	UFUNCTION(BlueprintCallable)
	float GetAttackSpeedInfluence() const { return AttackSpeedInfluence; }

	UFUNCTION(BlueprintCallable)
	float GetDamageInfluence() const { return DamageInfluence; }

	UFUNCTION(BlueprintCallable)
	float GetCooldownInfluence() const { return CooldownInfluence; }

	FNewStatusEffect EventNewStatusEffect;
	FRevertStatusEffect EventRevertStatusEffect;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<UStatusEffectBase*> CurrentStatusEffects;

	UPROPERTY()
	float MovementSpeedInfluence;

	UPROPERTY()
	float AttackSpeedInfluence;

	UPROPERTY()
	float DamageInfluence;

	UPROPERTY()
	float CooldownInfluence;

	//Status Effect Calculation Helpers
	void CalculateStatusEffectInfluence(UStatusEffectBase* StatusEffectData);
		
};
