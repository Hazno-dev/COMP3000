// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/ProjectileBase.h"
#include "Components/ActorComponent.h"
#include "ProjectileSpawner.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMP3000_API UProjectileSpawner : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectileSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<AProjectileBase> ProjectileClass;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(FVector Location, FRotator Rotation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ProjectileSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Damage = 10.f;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
		
};
