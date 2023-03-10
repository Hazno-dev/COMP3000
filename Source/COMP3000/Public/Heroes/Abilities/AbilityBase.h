// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilityBase.generated.h"

/**
 * 
 */
class AMainCharacter;

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
	void CoreEndCooldown(int ChargeIndex);

	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	virtual void StartCooldown();

	UFUNCTION(BlueprintCallable, Category = "ACore Functions")
	virtual void EndCooldown();

	//Definition Data
	UPROPERTY()
	bool bIsAbilityPlaying;

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

protected:
	UPROPERTY()
	AMainCharacter* MainCharacterRef;

	FTimerHandle AbilityEndTimerHandle;
	TMap<int32, FTimerHandle> CooldownTimerHandles;

	int32 CurrentChargeIndex = 0;

	UWorld* WorldRef;

	float BaseCharacterSpeed;

	float BaseCharacterAcceleration;
};
