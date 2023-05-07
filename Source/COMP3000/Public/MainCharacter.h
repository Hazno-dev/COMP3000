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
#include "GameFramework/PawnMovementComponent.h"
#include "Heroes/BaseAudioManager.h"
#include "Heroes/HeroManagerComponent.h"
#include "World/PSpawnPoint.h"
#include "World/WorldCursor.h"


#include "MainCharacter.generated.h"


class UPlayerAnimInstance;
class UCameraDynamicMotion;
class UPlayerBaseAbilities;
class UHeroGenerator;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmedToggle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFistFire);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCastingStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCastingEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCastingCancel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIconsSetup);
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

	/** Hero Manager */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Generation")
	UHeroManagerComponent* HeroManagerComponent;

	/** AISource */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionStimuliSourceComponent> AIStimuliSource;

	/** Projectile Spawner */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UProjectileSpawner* ProjectileSpawner;

	/* Image Capture Comp */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneCaptureComponent2D* ThumbnailCaptureComponent;

	FIconsSetup IconsSetup;

	UFUNCTION(BlueprintCallable)
	void ShootProjectile();

	/** Audio */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	USpringArmComponent* AudioListenerSpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	USceneComponent* AudioListenerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	UBaseAudioManager* AudioManager;

	UFUNCTION()
	void Footsteps();

	/* AI Collider Overlap, used for LODing the AI */
	UPROPERTY(VisibleAnywhere, Category = AICollider)
	TObjectPtr<USphereComponent> AICollider;
	
	UPROPERTY(VisibleAnywhere, Category = AICollider)
	TObjectPtr<USphereComponent> InnerAICollider;

	UFUNCTION()
	void OnAIColliderBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult);

	UFUNCTION()
	void OnAIColliderEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnInnerAIColliderBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);

	UFUNCTION()
	void OnInnerAIColliderEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* SwapHeroAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* CastAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* CancelCastAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Actions")
	UInputAction* OptionsAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Mappings")
	UInputMappingContext* BaseInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Mappings")
	UInputMappingContext* AOEInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Input Mappings")
	int32 BaseMappingPriority = 0;

	UEnhancedInputLocalPlayerSubsystem* Subsystem;


	//Pause Menu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pause Menu")
	TSubclassOf<UUserWidget> PauseMenuBP;

	UUserWidget* PauseMenu;
		
	UFUNCTION()
	void Pause();
	
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
	float FireTimer = 0.0f;

	/** Character Armed */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerController")
	bool Armed = false;

	/** Players rotation  */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerController")
	FRotator PublicRot = FRotator(0,0,0);

	//PUBLIC PLAYER STATS
	UFUNCTION()
	void TookDamage() const { HeroManagerComponent->TookDamage(); };

	UFUNCTION()
	void TookKillVolumeDamage () const { HeroManagerComponent->KillVolumeDamage(); };

	UFUNCTION()
	void GainXP(const int32 XP) const { HeroManagerComponent->AddXP(XP); };

	UFUNCTION()
	void SetSpawnPoint(APSpawnPoint* SpawnPoint) const { HeroManagerComponent->SetSpawnPoint(SpawnPoint); };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* DashEffect;

	//Crystal gaining
	UPROPERTY()
	bool bHasKeyCrystal;

	UFUNCTION()
	void SetHasKeyCrystal(bool bHasCrystal) { bHasKeyCrystal = bHasCrystal; };

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

	/** Casting Hold */
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* Mon_CastingHold;

	/** Casting Finish */
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* Mon_CastingFinish;
	
	FFistFire FistFire;
	FCastingStart CastingStart;
	FCastingEnd CastingFinish;
	FCastingCancel CastingCancel;

	/** Hitpoint for World Cursor*/
	FHitResult HitResultCursor;
	FVector LastHitLocationCursor;
	FVector_NetQuantizeNormal LastHitNormalCursor;

	
protected:

	// Class Overrides
	virtual void BeginPlay() override;
	
	/** Forward/Backwards motion */
	void MoveForward(float Value);

	/** Left/Right motion */
	void MoveRight(float Value);

	void EnhancedMove(const FInputActionValue& Value);

	/** Start punching */
	void Punch(const FInputActionInstance& ActionInstance);

	/** Stop punching */
	void StopPunch();

	/* Weapon Arm Toggle */
	void ToggleWeaponArm();

	/** Start Jumping */
	void JumpInternals();

private:

	/** Trace Channel for Mouse Aiming */
	ETraceTypeQuery TraceChannel;
	
	/** Hitpoint for Mouse Aiming */
	FHitResult HitResultAim;
	FVector LastHitLocationAim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimTraceChannel", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ETraceTypeQuery> TraceChannelAiming;

	UFUNCTION()
	FRotator GetAimRotation();

	UFUNCTION()
	void WorldCursorUpdate();

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

	//Dash Variables
	FVector DashVector;
	float HeldTime = 0.0f;

	float FootstepCounter = 0.0f;
};