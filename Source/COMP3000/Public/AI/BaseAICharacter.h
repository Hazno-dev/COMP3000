// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIGroupManager.h"
#include "ProjectileSpawner.h"
#include "Abilities/EnemyBaseAbilities.h"
#include "Abilities/EnemyStatusEffectSystem.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "Components/BoxComponent.h"
#include "Components/StateTreeComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "GameFramework/Character.h"
#include "BaseAICharacter.generated.h"

class ABaseAIController;
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Spawning,
	Idle,
	Detected,
	Chase,
	RangeAttack,
	MeleeAttack,
	Summoning,
	Dead
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventChangedAIState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTookDamageEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthChangedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDetectedEvent);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UEnemyBaseAbilities* EnemyBaseAbilitiesComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UEnemyStatusEffectSystem> EnemyStatusEffectSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* StatusBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* DetectionBoxFront;

	//Enum States
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EAIState AIState;
	
	UFUNCTION(BlueprintCallable, Category = "State")
	EAIState GetAIState() const {return AIState;};

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetAIState(const EAIState NewState) {AIState = NewState; EventChangedAIState.Broadcast();};

	UPROPERTY(BlueprintAssignable)
	FEventChangedAIState EventChangedAIState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health;
	float MaxHealth;

	//Drops
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 XPDeathReward;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops")
	TSubclassOf<class AXPOrb> XPOrbClass;

	UFUNCTION()
	void DeathDrops();

	//Health And Damage Functions
	UFUNCTION()
	float GetHealth() const {return Health;};

	UFUNCTION()
	float GetMaxHealth() const {return MaxHealth;};
	
	UFUNCTION()
	void ReceivedDamage(float Damage, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void DeathDestroy();

	UPROPERTY(BlueprintAssignable)
	FTookDamageEvent TookDamageEvent;
	FHealthChangedEvent HealthChangedEvent;

	//Melee Damage Properties
	UFUNCTION(BlueprintCallable)
	void MeleeDamageColliderEnabled(float Duration);

	UFUNCTION()
	void MeleeDamageColliderOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	bool bDealtMeleeDamage;
	
	//Ragdolling Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bImpulseable;
	
	UPROPERTY()
	bool bIsRagdoll;
	
	UFUNCTION(BlueprintCallable)
	void Ragdoll(float Duration);

	UFUNCTION()
	void UpdateRagdoll();

	UFUNCTION()
	void RecoverFromRagdoll();
	
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StopCurrentAnimation();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* HurtMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DeathMontage;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	AAIGroupManager* GetGroupManager() const {if(!IsValid(GroupManager)) return nullptr; return GroupManager;};

	UFUNCTION(BlueprintCallable)
	void SummonProjectile(int32 NumberOfProjectiles);

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* NewTarget);

	FDetectedEvent DetectedEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:
	// Helper function to predict the rotation of the projectile
	FRotator PredictProjectileRotation();

	void DeathStart();
	
	// Ability Functions
	// Damage Mitigation Checker
	bool TryMitigateDamage();

	// DeathCry checker
	bool TryDeathCry();

	ABaseAIController* ControllerRef;
};
