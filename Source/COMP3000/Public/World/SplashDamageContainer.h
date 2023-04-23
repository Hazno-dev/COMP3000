// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "SplashDamageContainer.generated.h"

UCLASS()
class COMP3000_API ASplashDamageContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplashDamageContainer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float BurstDamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float Age;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	USphereComponent* DamageSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	bool bDOT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (EditCondition = "bDOT"))
	float DOTDamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (EditCondition = "bDOT"))
	float DOTTickRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (EditCondition = "bDOT"))
	bool bBurstInitialDamageTimed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (EditCondition = "bDOT && bBurstInitialDamageTimed"))
	float BurstInitialDamageTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* NiagaraVFX;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void DOTDamage();
	
	UFUNCTION()
	void BurstInitialDamageDisable() { bInitialDamageTimeOver = true; }
	
	UFUNCTION()
	void DestroyContainer();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	FTimerHandle TimerHandle_DestroyAfterAge;
	FTimerHandle TimerHandle_DOTDamage;
	FTimerHandle TimerHandle_BurstInitialDamage;
	bool bInitialDamageTimeOver;

	TSet<AActor*> DamageBurstAppliedActors;
	TSet<AActor*> OverlappingActors;

};


