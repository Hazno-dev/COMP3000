// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIGroupManager.h"
#include "ProjectileSpawner.h"
#include "GameFramework/Character.h"
#include "BaseAICharacter.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Chase,
	RangeAttack,
	MeleeAttack,
	Dead
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventChangedAIState);

UCLASS()
class COMP3000_API ABaseAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAICharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	AAIGroupManager* GroupManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UProjectileSpawner* ProjectileSpawner;

	//Enum States
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EAIState AIState;
	
	UFUNCTION(BlueprintCallable, Category = "State")
	EAIState GetAIState() const {return AIState;};

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetAIState(const EAIState NewState) {AIState = NewState; EventChangedAIState.Broadcast();};

	UPROPERTY(BlueprintAssignable)
	FEventChangedAIState EventChangedAIState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	AAIGroupManager* GetGroupManager() const {if(!IsValid(GroupManager)) return nullptr; return GroupManager;};

	UFUNCTION(BlueprintCallable)
	void SummonProjectile();

private:
	FRotator PredictProjectileRotation();

};
