// To avoid direct asset ref in c++, use the blueprint class derived from this for changing mesh/data

#include "MainCharacter.h"

#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "COMP3000/COMP3000GameModeBase.h"
#include "COMP3000/COMP3000GameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Heroes/CameraDynamicMotion.h"
#include "Heroes/HeroGenerator.h"
#include "Heroes/PlayerBaseAbilities.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "PlayerAnimInstance.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initiate Camera + Arm components
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraDynamicMotion = CreateDefaultSubobject<UCameraDynamicMotion>(TEXT("CameraDynamicMotion"));
	AIStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIStimuliSource"));
	ProjectileSpawner = CreateDefaultSubobject<UProjectileSpawner>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PlayerBaseAbilitiesComponent = CreateDefaultSubobject<UPlayerBaseAbilities>(TEXT("PlayerBaseAbilities"));
	HeroGeneratorComponent = CreateDefaultSubobject<UHeroGenerator>(TEXT("HeroGenerator"));
	HeroManagerComponent = CreateDefaultSubobject<UHeroManagerComponent>(TEXT("HeroManager"));

	H_Left_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("H_Left_ParticleSystem"));
	H_Right_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("H_Right_ParticleSystem"));

	H_Left_ParticleSystem->SetupAttachment(GetMesh(), "HandLSocket");
	H_Right_ParticleSystem->SetupAttachment(GetMesh(), "HandRSocket");

	//Setup Hero Meshes
	//SetupMeshes();
	//HeroGeneratorComponent->SetupMeshes();
	
	// Collider Settings
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//Spring Arm Setup
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 30.0f), FRotator(-40.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.0f;
	//SpringArm->CameraLagMaxDistance = 1000.f;
	SpringArm->bDoCollisionTest = false;

	//Camera Setup
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	
	// No camera rotation!!
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	SpringArm->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	SpringArm->bInheritPitch = false; //Make sure spring arm isn't rotating based on player direction
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	//Character movement settings
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Assign + possess player Controller Index: 0
	SavedController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	SavedController->Possess(this);
	SavedController->bShowMouseCursor = true;
	SavedController->bEnableStreamingSource = true;
	SavedController->SetIsSpatiallyLoaded(true);

	//Assign TChannel for aiming
	TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	
	WorldCursor = GetWorld()->SpawnActor<AWorldCursor>(WorldCursorBP, FVector(0, 0, 0), FRotator(0, 0, 0));
	WorldCursor->ToggleVisibility(false);
	
	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(SavedController->GetLocalPlayer());
	
	ArmedToggle.Broadcast();

	HeroManagerComponent->SetupReferences(PlayerBaseAbilitiesComponent, HeroGeneratorComponent, this);

	TArray<UHeroDataInstance*> HeroDataArray = HeroGeneratorComponent->GenerateHero();
	if (HeroDataArray.Num() > 0) HeroManagerComponent->SetHeroDataArray(HeroDataArray);
}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PublicRot = GetAimRotation();
	SetActorRotation(PublicRot);
	
	WorldCursorUpdate();

	FireTimer += DeltaTime;
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!IsValid(PlayerBaseAbilitiesComponent)) return;
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (MovementAction) {
			PlayerEnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AMainCharacter::EnhancedMove);
		}
		if (JumpAction) {
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}
		if (FireAction) {
			PlayerEnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AMainCharacter::Punch);
			PlayerEnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AMainCharacter::StopPunch);
		}
		if (ArmedAction) {
			PlayerEnhancedInputComponent->BindAction(ArmedAction, ETriggerEvent::Started, this, &AMainCharacter::ToggleWeaponArm);
		}
		if (DashAction) {
			PlayerEnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, PlayerBaseAbilitiesComponent, &UPlayerBaseAbilities::StartDash);
		}
		if (Ability1Action) {
			PlayerEnhancedInputComponent->BindAction(Ability1Action, ETriggerEvent::Started, PlayerBaseAbilitiesComponent, &UPlayerBaseAbilities::StartAbility1);
		}
		if (Ability2Action) {
			PlayerEnhancedInputComponent->BindAction(Ability2Action, ETriggerEvent::Started, PlayerBaseAbilitiesComponent, &UPlayerBaseAbilities::StartAbility2);
		}
		if (Ability3Action) {
			PlayerEnhancedInputComponent->BindAction(Ability3Action, ETriggerEvent::Started, PlayerBaseAbilitiesComponent, &UPlayerBaseAbilities::StartAbility3);
		}
		if (UltimateAction)	{
			PlayerEnhancedInputComponent->BindAction(UltimateAction, ETriggerEvent::Started, PlayerBaseAbilitiesComponent, &UPlayerBaseAbilities::StartUltimate);
		}
		if (CastAction) {
			PlayerEnhancedInputComponent->BindAction(CastAction, ETriggerEvent::Started, PlayerBaseAbilitiesComponent, &UPlayerBaseAbilities::CastTriggerAnyAbility);
		}
		if (CancelCastAction) {
			PlayerEnhancedInputComponent->BindAction(CancelCastAction, ETriggerEvent::Started, PlayerBaseAbilitiesComponent, &UPlayerBaseAbilities::CastCancelAnyAbility);
		}
		if (SwapHeroAction) {
			PlayerEnhancedInputComponent->BindAction(SwapHeroAction, ETriggerEvent::Started, HeroManagerComponent, &UHeroManagerComponent::SwapHero);
		}
	}
}

void AMainCharacter::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
}

void AMainCharacter::PawnClientRestart() {
	Super::PawnClientRestart();

	if (APlayerController* PC = Cast<APlayerController>(GetController())) {
		Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (IsValid(Subsystem)) {
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(BaseInputMappingContext, BaseMappingPriority);
		}
	}
	
}

void AMainCharacter::SetHandParticlesOnL(bool On) {
	H_Left_ParticleSystem->SetVisibility(On);
	H_Left_ParticleSystem->SetActive(On);
}

void AMainCharacter::SetHandParticlesOnR(bool On) {
	H_Right_ParticleSystem->SetVisibility(On);
	H_Right_ParticleSystem->SetActive(On);
}


void AMainCharacter::ShootProjectile() {
	ProjectileSpawner->SpawnProjectile(ProjectileSpawner->GetComponentLocation(), GetActorRotation());
}


void AMainCharacter::MoveForward(float Value)
{
	if (Controller)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
 
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
 
void AMainCharacter::MoveRight(float Value)
{
	if (Controller)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
 	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::EnhancedMove(const FInputActionValue& Value) {
	
	if (!IsValid(Controller) || Value.GetMagnitude() == 0.f) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value[1]);
	AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value[0]);
}

void AMainCharacter::Punch(const FInputActionInstance& ActionInstance)
{
	if (PlayerBaseAbilitiesComponent->IsAnyAbilityPlayingOrCasting()) return;
	if (FireTimer < 0.56f) return;

	Punching = true;
	
	FireTimer = 0.f;;
	Armed = true;
	FistFire.Broadcast();
	SetHandParticlesOnL(true);
	SetHandParticlesOnR(true);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Punch"));
}

void AMainCharacter::StopPunch()
{
	Punching = false;
}

void AMainCharacter::ToggleWeaponArm() {
	if (PlayerBaseAbilitiesComponent->IsAnyAbilityPlayingOrCasting()) return;
	
	Armed = !Armed;
	ArmedToggle.Broadcast();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Armed: %d"), Armed));
}

FRotator AMainCharacter::GetAimRotation() {
	if (!IsValid(SavedController)) return FRotator::ZeroRotator;
	
	if (SavedController->GetHitResultUnderCursorByChannel(TraceChannelAiming, false, HitResultAim)) LastHitLocationAim = HitResultAim.Location;
	
	return FRotator(0, (UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LastHitLocationAim)).Yaw, 0);
	
}

void AMainCharacter::WorldCursorUpdate() {
	if (!IsValid(SavedController)) return;
	if (!IsValid(WorldCursor)) return;
	if (!IsValid(PlayerBaseAbilitiesComponent)) return;
	if (!PlayerBaseAbilitiesComponent->IsAnyAbilityCasting()) return;
	
	if (SavedController->GetHitResultUnderCursorByChannel(TraceChannel, true, HitResultCursor)) {
		LastHitLocationCursor = HitResultCursor.Location;
		LastHitNormalCursor = HitResultCursor.ImpactNormal;
	}

	WorldCursor->MoveCursor(HitResultCursor.Location, HitResultCursor.ImpactNormal);
	WorldCursor->ScaleCursor(PlayerBaseAbilitiesComponent->GetCurrentCastingAbility()->RangeCastSize);
}





