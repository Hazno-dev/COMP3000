// To avoid direct asset ref in c++, use the blueprint class derived from this for changing mesh/data

#include "MainCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initiate Camera + Arm components
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	AIStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIStimuliSource"));
	
	// Collider Settings
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//Spring Arm Setup
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 30.0f), FRotator(-40.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 60.0f;
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
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
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
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Rotate player to mouse
	SavedController->GetHitResultUnderCursorByChannel(TraceChannel, true, HitResult);
	PublicRot = FRotator(0, (UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), HitResult.Location).Yaw), 0);
	if (GetCharacterMovement()->Velocity.Size() > 0 && GetCharacterMovement()->GetCurrentAcceleration() != FVector::ZeroVector) SetActorRotation(PublicRot);
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

	PlayerInputComponent->BindAxis("Forward/Backward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Left/Right", this, &AMainCharacter::MoveRight);
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
}

void AMainCharacter::StopPunch()
{
	Punching = false;
}



