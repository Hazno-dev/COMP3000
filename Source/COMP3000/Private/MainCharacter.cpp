// To avoid direct asset ref in c++, use the blueprint class derived from this for changing mesh/data

#include "MainCharacter.h"

#include "TimerManager.h"
#include "Camera/CameraComponent.h"
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

	//Assign TChannel for aiming
	TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	
	WorldCursor = GetWorld()->SpawnActor<AWorldCursor>(WorldCursorBP, FVector(0, 0, 0), FRotator(0, 0, 0));

	ArmedToggle.Broadcast();
}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Rotate player to mouse
	if (SavedController->GetHitResultUnderCursorByChannel(TraceChannel, true, HitResult)) {
		PublicRot = FRotator(0, (UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), HitResult.Location).Yaw), 0);
		if (GetCharacterMovement()->Velocity.Size() > 0 && GetCharacterMovement()->GetCurrentAcceleration() != FVector::ZeroVector) SetActorRotation(PublicRot);
		WorldCursor->MoveCursor(HitResult.Location, HitResult.ImpactNormal);
	}

	HeldKeyManager(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &AMainCharacter::Punch);
	PlayerInputComponent->BindAction("Punch", IE_Released, this, &AMainCharacter::StopPunch);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacter::StartDash);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &AMainCharacter::EndDash);
	PlayerInputComponent->BindAction("Armed", IE_Pressed, this, &AMainCharacter::ToggleWeaponArm);

	PlayerInputComponent->BindAxis("Forward/Backward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Left/Right", this, &AMainCharacter::MoveRight);
}

void AMainCharacter::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
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

void AMainCharacter::Punch()
{
	if (!Punching) Punching = true;
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

void AMainCharacter::StartDash() {

	/*
	FTimerHandle DelayHandle;
	WorldCursor->ToggleVisibility(false);
	
	DashVector = (GetCharacterMovement()->Velocity.GetSafeNormal().IsNearlyZero())
		? GetActorForwardVector() : GetCharacterMovement()->Velocity.GetSafeNormal();
		*/

	PlayerBaseAbilitiesComponent->StartDash();
	
	//GetMesh()->SetHiddenInGame(true, true);
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DashEffect, GetActorLocation(), GetActorRotation());
	//GetCharacterMovement()->Deactivate();
	
	//GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &AMainCharacter::EndDash, 0.1f, false);
	//EndDash();
}

void AMainCharacter::HeldDash(float DeltaTime) {

	if (GetCharacterMovement()->Velocity.GetSafeNormal().IsNearlyZero()) {
		DashVector = GetActorForwardVector();
		WorldCursor->ToggleVisibility(true);
	} else {
		DashVector = GetCharacterMovement()->Velocity.GetSafeNormal();
	}
}

void AMainCharacter::EndDash() {
	
	//HeldTime = 0.0f;
	PlayerBaseAbilitiesComponent->EndDash();
	
	//GetMesh()->SetHiddenInGame(true, true);
	/*UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DashEffect, GetActorLocation(), GetActorRotation());
	//GetCharacterMovement()->Deactivate();*/
	//CameraDynamicMotion->SetCameraLag(1.f);
	
	//GetMesh()->SetHiddenInGame(false, true);
	//GetMesh()->SetHiddenInGame(true, false);
	
	/*
	WorldCursor->ToggleVisibility(false);
	
	//GetCharacterMovement()->Activate();
	//GetCamera()->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetCamera()->GetComponentLocation(), GetActorLocation()));
	FVector DashPosition = GetActorLocation() + DashVector * 1000;
	SetActorLocation(DashPosition, true, nullptr, ETeleportType::TeleportPhysics);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DashEffect, GetActorLocation(), GetActorRotation());
	*/

	//SpringArm->CameraLagMaxDistance = 0.0f;
	
}

void AMainCharacter::ToggleWeaponArm() {
	Armed = !Armed;
	ArmedToggle.Broadcast();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Armed: %d"), Armed));
}

/* Iterate through required held key events, rather than using IE_Repeat (laggy, sucks) */
void AMainCharacter::HeldKeyManager(float DeltaTime) {
	if (SavedController->IsInputKeyDown(EKeys::LeftShift)) {
		HeldDash(DeltaTime);
	}
}





