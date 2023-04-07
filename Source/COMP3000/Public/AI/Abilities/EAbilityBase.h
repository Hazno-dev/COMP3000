// Callum Stables, 2023
// EAbilityBase is a replication of AbilityBase (ability for player) with modifications to suit AI characters.
// This is the base class for all enemy abilities.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Heroes/Abilities/AbilityData.h"
#include "World/FadeText.h"
#include "EAbilityBase.generated.h"

class ABaseAICharacter;

UCLASS()
class COMP3000_API AEAbilityBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEAbilityBase();
	
	virtual void CustomTick(float DeltaTime);

	virtual void CustomBeginPlay(ABaseAICharacter* AICharacter, UWorld* World, TSubclassOf<AFadeText> InFadeTextClass = nullptr);

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

	//Interruption Functions
	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	void CoreInterruptAbility();

	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	virtual void InterruptAbility();

	//Definition Data
	UPROPERTY()
	bool bIsAbilityPlaying;

	UPROPERTY()
	bool bIsInterrupted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data")
	TEnumAsByte<EAbilityType> AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data")
	bool bCanBeInterrupted;

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
	
protected:
	UPROPERTY()
	ABaseAICharacter* AICharacterRef;

	FTimerHandle AbilityEndTimerHandle;
	FTimerHandle CooldownTimerHandle;

	int32 CurrentChargeIndex = 0;

	UWorld* WorldRef;

	float BaseCharacterSpeed;

	float BaseCharacterAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data")
	TSubclassOf<AFadeText>  FadeTextClass;

};
