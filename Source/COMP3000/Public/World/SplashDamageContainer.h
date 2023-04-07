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
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float Age;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	USphereComponent* DamageSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* NiagaraVFX;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void DestroyContainer();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	FTimerHandle TimerHandle_DestroyAfterAge;

};
