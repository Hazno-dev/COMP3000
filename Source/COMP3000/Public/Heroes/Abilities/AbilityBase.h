// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Heroes/Abilities/AbilityData.h"
#include "AbilityBase.generated.h"

/**
 * 
 */
class AMainCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityCooldownStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityCooldownEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbiltiyStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityEnd);
UCLASS()
class COMP3000_API AAbilityBase : public AActor
{
	GENERATED_BODY()

public:
	AAbilityBase();
	
	virtual void CustomTick(float DeltaTime);

	virtual void CustomBeginPlay(AMainCharacter* MainCharacter, UWorld* World);

	//Usage Functions
	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	void CoreBeginAbility();

	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	void CoreEndAbility();
	
	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	virtual void BeginAbility();

	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	virtual void PlayingAbility();

	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	virtual void EndAbility();

	//Recharge Functions
	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	void CoreStartCooldown();

	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	void CoreEndCooldown();
	
	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	virtual void StartCooldown();

	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	virtual void EndCooldown();

	//Definition Data
	UPROPERTY()
	bool bIsAbilityPlaying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data")
	TEnumAsByte<EAbilityType> AbilityType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data")
	bool aSyncRecharge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data")
	int32 MaxCharges;

	int32 CurrentCharges;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data")
	float AbilityRechargeDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data")
	float AbilityDuration;

	UFUNCTION(BlueprintCallable, Category = "Ability Data")
	void SetAbilityRechargeDuration(float Cooldown) { AbilityRechargeDuration = Cooldown; };

	UFUNCTION(BlueprintCallable, Category = "Ability Data")
	void SetAbilityDuration(float Duration) { AbilityDuration = Duration; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	UTexture2D* AbilityIcon;
	

	FAbilityCooldownStart OnAbilityCooldownStart;
	FAbilityCooldownEnd OnAbilityCooldownEnd;
	FAbiltiyStart OnAbilityStart;
	FAbilityEnd OnAbilityEnd;
	
protected:
	UPROPERTY()
	AMainCharacter* MainCharacterRef;

	FTimerHandle AbilityEndTimerHandle;
	FTimerHandle CooldownTimerHandle;

	int32 CurrentChargeIndex = 0;

	UWorld* WorldRef;

	float BaseCharacterSpeed;

	float BaseCharacterAcceleration;
};
