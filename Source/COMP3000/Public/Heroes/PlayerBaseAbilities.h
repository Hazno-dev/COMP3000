// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/ActorComponent.h"
#include "Heroes/Abilities/AB_Dash.h"
#include "Heroes/Abilities/AbilityData.h"
#include "PlayerBaseAbilities.generated.h"

class AMainCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRefreshAbilityIcons);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMP3000_API UPlayerBaseAbilities : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerBaseAbilities();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	TSubclassOf<AAbilityBase> DashClass;

	UPROPERTY()
	AAbilityBase* DashAbilityInstance;

	UPROPERTY()
	AAbilityBase* Ability1Instance;

	UPROPERTY()
	AAbilityBase* Ability2Instance;

	UPROPERTY()
	AAbilityBase* Ability3Instance;

	UPROPERTY()
	AAbilityBase* UltimateAbilityInstance;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void StartDash();

	UFUNCTION()
	void StartAbility1();

	UFUNCTION()
	void StartAbility2();

	UFUNCTION()
	void StartAbility3();

	UFUNCTION()
	void StartUltimate();

	UFUNCTION()
	AAbilityBase* AbilityFromEnum(const EAbilitySlotType AbilitySlotType) const;

	UFUNCTION()
	void SetDashMaxCharges(const int32 NewCharges);

	UFUNCTION()
	void SetDashDuration(const float NewDuration);

	UFUNCTION()
	void SetDashVFX(UNiagaraSystem* Explosion, UNiagaraSystem* Trail);

	UFUNCTION()
	void SetAbility1Instance(AAbilityBase* NewAbility);

	UFUNCTION()
	void SetAbility2Instance(AAbilityBase* NewAbility);

	UFUNCTION()
	void SetAbility3Instance(AAbilityBase* NewAbility);

	UFUNCTION()
	void SetUltimateAbilityInstance(AAbilityBase* NewAbility);

	UFUNCTION()
	void CastTriggerAnyAbility();

	UFUNCTION()
	void CastCancelAnyAbility();

	UFUNCTION(BlueprintCallable)
	bool IsAnyAbilityPlayingOrCasting(bool bIgnoreCastingPlaying = true) const;

	UFUNCTION(BlueprintCallable)
	bool IsAnyAbilityCasting() const;

	UFUNCTION()
	AAbilityBase* GetCurrentCastingAbility() const;

	FRefreshAbilityIcons RefreshAbilityIcons;

private:

	UPROPERTY()
	AMainCharacter* MainCharacterRef;
};
