// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileSpawner.h"
#include "GameFramework/Character.h"
#include "Heroes/HeroGeneration.h"
//#include "Heroes/HeroGenerator.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/PointLightComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "World/WorldCursor.h"


#include "MainCharacter.generated.h"


class UCameraDynamicMotion;
class UPlayerBaseAbilities;
class UHeroGenerator;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmedToggle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFistFire);
UCLASS()
class COMP3000_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	//UNREAL DEFAULTS
	AMainCharacter();
	
	// Class Overrides
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PawnClientRestart() override;
	
	/*
	 * Component Definitions
	 */
	
	/** Main Camera */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	UFUNCTION()
	UCameraComponent* GetCamera() const { return Camera; }
	
	/** Camera Spring Arm */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

	UFUNCTION()
	USpringArmComponent* GetSpringArm() const { return SpringArm; }

	/** Camera Dynamic Motion*/
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraDynamicMotion* CameraDynamicMotion;

	/** Hero Generator */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Generation")
	UHeroGenerator* HeroGeneratorComponent;

	/** AISource */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionStimuliSourceComponent> AIStimuliSource;

	/** Projectile Spawner */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UProjectileSpawner* ProjectileSpawner;

	UFUNCTION(BlueprintCallable)
	void ShootProjectile();
	
	/*
	 * Ability System
	 */

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities")
	UPlayerBaseAbilities* PlayerBaseAbilitiesComponent;

	/** Hand Particle System */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities")
	UParticleSystemComponent* H_Left_ParticleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities")
	UParticleSystemComponent* H_Right_ParticleSystem;

	UFUNCTION(BlueprintCallable)
	void SetHandParticlesOnL(bool On);

	UFUNCTION(BlueprintCallable)
	void SetHandParticlesOnR(bool On);

	/**
	 * Inputs
	 */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* MovementAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* ArmedAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* DashAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* Ability1Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* Ability2Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* Ability3Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* UltimateAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Mappings")
	UInputMappingContext* BaseInputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Mappings")
	int32 BaseMappingPriority = 0;

	UEnhancedInputLocalPlayerSubsystem* Subsystem;
	
	/**
	 * World Cursor
	 */

	//Cursor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	TSubclassOf<AWorldCursor> WorldCursorBP;

	UPROPERTY()
	TObjectPtr<AWorldCursor> WorldCursor;
	
	/*
	 * Player Data Variables
	 */
	
	/** Character holding punch */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerController")
	bool Punching = false;
	UPROPERTY(BlueprintReadWrite, Category = "PlayerController")
	bool CanFire = true;

	/** Character Armed */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerController")
	bool Armed = false;

	/** Players rotation  */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerController")
	FRotator PublicRot = FRotator(0,0,0);

	//PUBLIC PLAYER STATS
	/** Players Health */
	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	int PlayerHealth;

	//Heroes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroes")
	TArray<FHeroDataStruct> HeroDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* DashEffect;

	/*
	 * References
	 */
	
	/** Player Controller reference */
	UPROPERTY()
	APlayerController* SavedController;

	/*
	 * Asset References 
	 */
	
	/** Punching Montage  */
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* Mon_Punching;

	/** Punching Montage  */
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* Mon_Walking;

	/** Enter Armed Montage */
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* Mon_EnterArmed;
	FArmedToggle ArmedToggle;
	
	/** Exit Armed Montage */
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* Mon_ExitArmed;

	/** Fire Fist Montage Left */
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* Mon_FireFistLeft;

	/** Fire Fist Montage Right */
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* Mon_FireFistRight;
	
	FFistFire FistFire;
	
	
protected:

	// Class Overrides
	virtual void BeginPlay() override;
	
	/** Forward/Backwards motion */
	void MoveForward(float Value);

	/** Left/Right motion */
	void MoveRight(float Value);

	void EnhancedMove(const FInputActionValue& Value);

	/** Start punching */
	void Punch();

	/** Stop punching */
	void StopPunch();

	/* Dash */
	void StartDash();

	void HeldDash(float DeltaTime);

	void EndDash();

	/* Weapon Arm Toggle */
	void ToggleWeaponArm();

private:

	/** Trace Channel for Mouse Aiming */
	ETraceTypeQuery TraceChannel;
	
	/** Hitpoint for Mouse Aiming */
	FHitResult HitResult;

	UFUNCTION()
	void HeldKeyManager(float DeltaTime);

	/*UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetupMeshes();
	
	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void GenerateHero(UPARAM(ref) FHeroDataStruct& InHero);

	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetMaterials();

	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetMaterialParameters(FHeroDataStruct InHero);

	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetMeshes(FHeroDataStruct InHero);*/

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

	//Dash Variables
	FVector DashVector;
	float HeldTime = 0.0f;
};
