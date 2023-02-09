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
	AIStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIStimuliSource"));
	ProjectileSpawner = CreateDefaultSubobject<UProjectileSpawner>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//HeroMesh Components
	AHeadCoveringNoHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHeadCoveringNoHair"));
	AHeadCoveringNoHair->SetupAttachment(GetMesh());

	AHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHair"));
	AHair->SetupAttachment(GetMesh());

	AHeadAttachmentHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHeadAttachmentHair"));
	AHeadAttachmentHair->SetupAttachment(GetMesh());

	AHeadAttachmentHelmet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHeadAttachmentHelmet"));
	AHeadAttachmentHelmet->SetupAttachment(GetMesh());

	AChestAttachment = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AChestAttachment"));
	AChestAttachment->SetupAttachment(GetMesh());

	AHeadCoveringBaseHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHeadCoveringBaseHair"));
	AHeadCoveringBaseHair->SetupAttachment(GetMesh());

	AHeadCoveringNoFacialHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHeadCoveringNoFacialHair"));
	AHeadCoveringNoFacialHair->SetupAttachment(GetMesh());

	ABackAttachment = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ABackAttachment"));
	ABackAttachment->SetupAttachment(GetMesh());

	AShoulderAttachmentL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AShoulderAttachmentL"));
	AShoulderAttachmentL->SetupAttachment(GetMesh());

	AShoulderAttachmentR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AShoulderAttachmentR"));
	AShoulderAttachmentR->SetupAttachment(GetMesh());

	AElbowAttachmentL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AElbowAttachmentL"));
	AElbowAttachmentL->SetupAttachment(GetMesh());

	AElbowAttachmentR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AElbowAttachmentR"));
	AElbowAttachmentR->SetupAttachment(GetMesh());

	AHipAttachment = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHipAttachment"));
	AHipAttachment->SetupAttachment(GetMesh());

	AKneeAttachmentL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AKneeAttachmentL"));
	AKneeAttachmentL->SetupAttachment(GetMesh());

	AKneeAttachmentR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AKneeAttachmentR"));
	AKneeAttachmentR->SetupAttachment(GetMesh());

	AExtra	= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AExtra"));
	AExtra->SetupAttachment(GetMesh());

	GLegL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GLegL"));
	GLegL->SetupAttachment(GetMesh());

	GLegR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GLegR"));
	GLegR->SetupAttachment(GetMesh());

	GHead = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GHead"));
	GHead->SetupAttachment(GetMesh());

	GEyebrows = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GEyebrows"));
	GEyebrows->SetupAttachment(GetMesh());

	GFacialHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GFacialHair"));
	GFacialHair->SetupAttachment(GetMesh());

	GTorso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GTorso"));
	GTorso->SetupAttachment(GetMesh());

	GArmUpperR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GArmUpperR"));
	GArmUpperR->SetupAttachment(GetMesh());

	GArmUpperL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GArmUpperL"));
	GArmUpperL->SetupAttachment(GetMesh());

	GArmLowerR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GArmLowerR"));
	GArmLowerR->SetupAttachment(GetMesh());

	GArmLowerL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GArmLowerL"));
	GArmLowerL->SetupAttachment(GetMesh());

	GHandR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GHandR"));
	GHandR->SetupAttachment(GetMesh());

	GHandL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GHandL"));
	GHandL->SetupAttachment(GetMesh());

	GHips = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GHips"));
	GHips->SetupAttachment(GetMesh());
	
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

	GenerateHero(HeroDataArray[0]);
	SetMeshes(HeroDataArray[0]);
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
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacter::StartDash);

	PlayerInputComponent->BindAxis("Forward/Backward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Left/Right", this, &AMainCharacter::MoveRight);
}

void AMainCharacter::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	
	ABackAttachment->SetupAttachment(GetMesh(), "spine_03");
	
	DynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(0, MaterialReference);
	SetMaterials();
	
	GenerateHero(HeroDataArray[0]);
	SetMeshes(HeroDataArray[0]);
	
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
}

void AMainCharacter::StopPunch()
{
	Punching = false;
}

void AMainCharacter::StartDash() {

	FTimerHandle DelayHandle;

	DashVector = GetActorForwardVector();
	GetMesh()->SetHiddenInGame(true, true);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DashEffect, GetActorLocation(), GetActorRotation());
	GetCharacterMovement()->Deactivate();
	
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &AMainCharacter::EndDash, 0.4f, false);
}

void AMainCharacter::EndDash() {
	GetMesh()->SetHiddenInGame(false, true);
	GetMesh()->SetHiddenInGame(true, false);

	GetCharacterMovement()->Activate();
	FVector DashPosition = GetActorLocation() + DashVector * 1000;
	SetActorLocation(DashPosition, true);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DashEffect, GetActorLocation(), GetActorRotation());

	
}


void AMainCharacter::GenerateHero(UPARAM(ref) FHeroDataStruct& InHero) {
	//Gendered Data
	InHero.Gender = UKismetMathLibrary::RandomBool();
	
	InHero.GHeadInt = UKismetMathLibrary::RandomIntegerInRange(0, MHeadArray.Num() - 1);

	if (InHero.Gender) {
		InHero.GEyebrowsInt = UKismetMathLibrary::RandomIntegerInRange(0, MEyebrowsArray.Num() - 1);
		InHero.GFacialHairInt = UKismetMathLibrary::RandomIntegerInRange(0, MFacialHairArray.Num() - 1);
	}
	else {
		InHero.GEyebrowsInt = UKismetMathLibrary::RandomIntegerInRange(0, FEyebrowsArray.Num() - 1);
		InHero.GFacialHairInt = 0;
	}

	InHero.GTorsoInt = UKismetMathLibrary::RandomIntegerInRange(0, MTorsoArray.Num() - 1);
	
	InHero.GUpperArmLInt = UKismetMathLibrary::RandomIntegerInRange(0, MUpperArmLArray.Num() - 1);
	UKismetMathLibrary::RandomBoolWithWeight(0.8) ? InHero.GUpperArmRInt = InHero.GUpperArmLInt :
		InHero.GUpperArmRInt = UKismetMathLibrary::RandomIntegerInRange(0, MUpperArmRArray.Num() - 1);

	InHero.GLowerArmLInt = UKismetMathLibrary::RandomIntegerInRange(0, MLowerArmLArray.Num() - 1);
	UKismetMathLibrary::RandomBoolWithWeight(0.8) ? InHero.GLowerArmRInt = InHero.GLowerArmLInt :
		InHero.GLowerArmRInt = UKismetMathLibrary::RandomIntegerInRange(0, MLowerArmRArray.Num() - 1);

	InHero.GHandLInt = UKismetMathLibrary::RandomIntegerInRange(0, MHandLArray.Num() - 1);
	UKismetMathLibrary::RandomBoolWithWeight(0.8) ? InHero.GHandRInt = InHero.GHandLInt :
		InHero.GHandRInt = UKismetMathLibrary::RandomIntegerInRange(0, MHandRArray.Num() - 1);

	InHero.GHipsInt = UKismetMathLibrary::RandomIntegerInRange(0, MHipsArray.Num() - 1);

	InHero.GLegLInt = UKismetMathLibrary::RandomIntegerInRange(0, MLegLArray.Num() - 1);
	UKismetMathLibrary::RandomBoolWithWeight(0.8) ? InHero.GLegRInt = InHero.GLegLInt :
		InHero.GLegRInt = UKismetMathLibrary::RandomIntegerInRange(0, MLegRArray.Num() - 1);

	// Generic Data
	InHero.HeadCoveringInt = UKismetMathLibrary::RandomIntegerInRange(0, 3);
	
	InHero.AHeadCoverBaseHairInt = UKismetMathLibrary::RandomIntegerInRange(0, AHeadCoverBaseHairArray.Num() - 1);

	InHero.AHeadCoverNoFacialHairInt = UKismetMathLibrary::RandomIntegerInRange(0, AHeadCoverNoFacialHairArray.Num() - 1);

	InHero.AHeadCoverNoHairInt = UKismetMathLibrary::RandomIntegerInRange(0, AHeadCoverNoHairArray.Num() - 1);

	InHero.AHairInt = UKismetMathLibrary::RandomIntegerInRange(0, AHairArray.Num() - 1);

	InHero.AHeadAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AHeadAttachmentHelmetArray.Num() - 1);

	InHero.ABackAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, ABackAttachmentArray.Num() - 1);

	InHero.AShoulderLAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AShoulderLAttachmentArray.Num() - 1);

	InHero.AShoulderRAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AShoulderRAttachmentArray.Num() - 1);

	InHero.AElbowLAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AElbowLAttachmentArray.Num() - 1);

	InHero.AElbowRAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AElbowRAttachmentArray.Num() - 1);

	InHero.AHipsAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AHipsAttachmentArray.Num() - 1);

	InHero.AKneeLAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AKneeLAttachmentArray.Num() - 1);

	InHero.AKneeRAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AKneeRAttachmentArray.Num() - 1);

	UKismetMathLibrary::RandomBoolWithWeight(0.8) ? InHero.AExtraInt = 0 :
		InHero.AExtraInt = UKismetMathLibrary::RandomIntegerInRange(0, AExtraArray.Num() - 1);

	//Colour Data
	InHero.SkinColourInt = UKismetMathLibrary::RandomIntegerInRange(0, SkinColourArray.Num() - 1);

	InHero.PrimaryColourInt = UKismetMathLibrary::RandomIntegerInRange(0, PrimaryColourArray.Num() - 1);

	InHero.SecondaryColourInt = UKismetMathLibrary::RandomIntegerInRange(0, SecondaryColourArray.Num() - 1);

	InHero.MetalPrimaryColourInt = UKismetMathLibrary::RandomIntegerInRange(0, MetalPrimaryColourArray.Num() - 1);

	InHero.MetalSecondaryColourInt = UKismetMathLibrary::RandomIntegerInRange(0, MetalSecondaryColourArray.Num() - 1);

	InHero.MetalDarkColourInt = UKismetMathLibrary::RandomIntegerInRange(0, MetalDarkColourArray.Num() - 1);

	InHero.LeatherPrimaryColourInt = UKismetMathLibrary::RandomIntegerInRange(0, LeatherPrimaryColourArray.Num() - 1);

	InHero.LeatherSecondaryColourInt = UKismetMathLibrary::RandomIntegerInRange(0, LeatherSecondaryColourArray.Num() - 1);

	InHero.HairColourInt = UKismetMathLibrary::RandomIntegerInRange(0, HairColourArray.Num() - 1);

	InHero.BodyArtColourInt = UKismetMathLibrary::RandomIntegerInRange(0, BodyArtColourArray.Num() - 1);
}

void AMainCharacter::SetMaterials() {
	GHead->SetMaterial(0, DynamicMaterial);
	GEyebrows->SetMaterial(0, DynamicMaterial);
	GFacialHair->SetMaterial(0, DynamicMaterial);
	GTorso->SetMaterial(0, DynamicMaterial);
	GArmUpperL->SetMaterial(0, DynamicMaterial);
	GArmUpperR->SetMaterial(0, DynamicMaterial);
	GArmLowerL->SetMaterial(0, DynamicMaterial);
	GArmLowerR->SetMaterial(0, DynamicMaterial);
	GHandL->SetMaterial(0, DynamicMaterial);
	GHandR->SetMaterial(0, DynamicMaterial);
	GHips->SetMaterial(0, DynamicMaterial);
	GLegL->SetMaterial(0, DynamicMaterial);
	GLegR->SetMaterial(0, DynamicMaterial);
	AHeadCoveringNoHair->SetMaterial(0, DynamicMaterial);
	AHair->SetMaterial(0, DynamicMaterial);
	AHeadAttachmentHair->SetMaterial(0, DynamicMaterial);
	AHeadAttachmentHelmet->SetMaterial(0, DynamicMaterial);
	AChestAttachment->SetMaterial(0, DynamicMaterial);
	AHeadCoveringBaseHair->SetMaterial(0, DynamicMaterial);
	AHeadCoveringNoFacialHair->SetMaterial(0, DynamicMaterial);
	ABackAttachment->SetMaterial(0, DynamicMaterial);
	AShoulderAttachmentL->SetMaterial(0, DynamicMaterial);
	AShoulderAttachmentR->SetMaterial(0, DynamicMaterial);
	AElbowAttachmentL->SetMaterial(0, DynamicMaterial);
	AElbowAttachmentR->SetMaterial(0, DynamicMaterial);
	AHipAttachment->SetMaterial(0, DynamicMaterial);
	AKneeAttachmentL->SetMaterial(0, DynamicMaterial);
	AKneeAttachmentR->SetMaterial(0, DynamicMaterial);
	AExtra->SetMaterial(0, DynamicMaterial);
}

void AMainCharacter::SetMaterialParameters(FHeroDataStruct InHero) {
	
	DynamicMaterial->SetVectorParameterValue("Color_Skin",
		SkinColourArray[UKismetMathLibrary::ClampInt64(InHero.SkinColourInt, 0, SkinColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Stubble",
		StubbleColourArray[UKismetMathLibrary::ClampInt64(InHero.SkinColourInt, 0, StubbleColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Scar",
		ScarsColourArray[UKismetMathLibrary::ClampInt64(InHero.SkinColourInt, 0, ScarsColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Primary",
		PrimaryColourArray[UKismetMathLibrary::ClampInt64(InHero.PrimaryColourInt, 0, PrimaryColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Secondary",
		SecondaryColourArray[UKismetMathLibrary::ClampInt64(InHero.SecondaryColourInt, 0, SecondaryColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Metal_Primary",
		MetalPrimaryColourArray[UKismetMathLibrary::ClampInt64(InHero.MetalPrimaryColourInt, 0, MetalPrimaryColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Metal_Secondary",
		MetalSecondaryColourArray[UKismetMathLibrary::ClampInt64(InHero.MetalSecondaryColourInt, 0, MetalSecondaryColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Metal_Dark",
		MetalDarkColourArray[UKismetMathLibrary::ClampInt64(InHero.MetalDarkColourInt, 0, MetalDarkColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Leather_Primary",
		LeatherPrimaryColourArray[UKismetMathLibrary::ClampInt64(InHero.LeatherPrimaryColourInt, 0, LeatherPrimaryColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Leather_Secondary",
		LeatherSecondaryColourArray[UKismetMathLibrary::ClampInt64(InHero.LeatherSecondaryColourInt, 0, LeatherSecondaryColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Hair",
		HairColourArray[UKismetMathLibrary::ClampInt64(InHero.HairColourInt, 0, HairColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_BodyArt",
		BodyArtColourArray[UKismetMathLibrary::ClampInt64(InHero.BodyArtColourInt, 0, BodyArtColourArray.Num() - 1)]);
	
}

void AMainCharacter::SetMeshes(FHeroDataStruct InHero) {
	GetMesh()->SetMaterial(0, MaskReference);

	SetMaterialParameters(InHero);

	if (InHero.Gender) {
		GHead->SetSkeletalMesh(MHeadArray[UKismetMathLibrary::Clamp(InHero.GHeadInt, 0, MHeadArray.Num() - 1)]);
		GEyebrows->SetSkeletalMesh(MEyebrowsArray[UKismetMathLibrary::Clamp(InHero.GEyebrowsInt, 0, MEyebrowsArray.Num() - 1)]);
		GFacialHair->SetSkeletalMesh(MFacialHairArray[UKismetMathLibrary::Clamp(InHero.GFacialHairInt, 0, MFacialHairArray.Num() - 1)]);
		GTorso->SetSkeletalMesh(MTorsoArray[UKismetMathLibrary::Clamp(InHero.GTorsoInt, 0, MTorsoArray.Num() - 1)]);
		GArmUpperL->SetSkeletalMesh(MUpperArmLArray[UKismetMathLibrary::Clamp(InHero.GUpperArmLInt, 0, MUpperArmLArray.Num() - 1)]);
		GArmUpperR->SetSkeletalMesh(MUpperArmRArray[UKismetMathLibrary::Clamp(InHero.GUpperArmRInt, 0, MUpperArmRArray.Num() - 1)]);
		GArmLowerL->SetSkeletalMesh(MLowerArmLArray[UKismetMathLibrary::Clamp(InHero.GLowerArmLInt, 0, MLowerArmLArray.Num() - 1)]);
		GArmLowerR->SetSkeletalMesh(MLowerArmRArray[UKismetMathLibrary::Clamp(InHero.GLowerArmRInt, 0, MLowerArmRArray.Num() - 1)]);
		GHandL->SetSkeletalMesh(MHandLArray[UKismetMathLibrary::Clamp(InHero.GHandLInt, 0, MHandLArray.Num() - 1)]);
		GHandR->SetSkeletalMesh(MHandRArray[UKismetMathLibrary::Clamp(InHero.GHandRInt, 0, MHandRArray.Num() - 1)]);
		GHips->SetSkeletalMesh(MHipsArray[UKismetMathLibrary::Clamp(InHero.GHipsInt, 0, MHipsArray.Num() - 1)]);
		GLegL->SetSkeletalMesh(MLegLArray[UKismetMathLibrary::Clamp(InHero.GLegLInt, 0, MLegLArray.Num() - 1)]);
		GLegR->SetSkeletalMesh(MLegRArray[UKismetMathLibrary::Clamp(InHero.GLegRInt, 0, MLegRArray.Num() - 1)]);
	} else {
		GHead->SetSkeletalMesh(FHeadArray[UKismetMathLibrary::Clamp(InHero.GHeadInt, 0, FHeadArray.Num() - 1)]);
		GEyebrows->SetSkeletalMesh(FEyebrowsArray[UKismetMathLibrary::Clamp(InHero.GEyebrowsInt, 0, FEyebrowsArray.Num() - 1)]);
		GTorso->SetSkeletalMesh(FTorsoArray[UKismetMathLibrary::Clamp(InHero.GTorsoInt, 0, FTorsoArray.Num() - 1)]);
		GArmUpperL->SetSkeletalMesh(FUpperArmLArray[UKismetMathLibrary::Clamp(InHero.GUpperArmLInt, 0, FUpperArmLArray.Num() - 1)]);
		GArmUpperR->SetSkeletalMesh(FUpperArmRArray[UKismetMathLibrary::Clamp(InHero.GUpperArmRInt, 0, FUpperArmRArray.Num() - 1)]);
		GArmLowerL->SetSkeletalMesh(FLowerArmLArray[UKismetMathLibrary::Clamp(InHero.GLowerArmLInt, 0, FLowerArmLArray.Num() - 1)]);
		GArmLowerR->SetSkeletalMesh(FLowerArmRArray[UKismetMathLibrary::Clamp(InHero.GLowerArmRInt, 0, FLowerArmRArray.Num() - 1)]);
		GHandL->SetSkeletalMesh(FHandLArray[UKismetMathLibrary::Clamp(InHero.GHandLInt, 0, FHandLArray.Num() - 1)]);
		GHandR->SetSkeletalMesh(FHandRArray[UKismetMathLibrary::Clamp(InHero.GHandRInt, 0, FHandRArray.Num() - 1)]);
		GHips->SetSkeletalMesh(FHipsArray[UKismetMathLibrary::Clamp(InHero.GHipsInt, 0, FHipsArray.Num() - 1)]);
		GLegL->SetSkeletalMesh(FLegLArray[UKismetMathLibrary::Clamp(InHero.GLegLInt, 0, FLegLArray.Num() - 1)]);
		GLegR->SetSkeletalMesh(FLegRArray[UKismetMathLibrary::Clamp(InHero.GLegRInt, 0, FLegRArray.Num() - 1)]);
	}
	switch (InHero.HeadCoveringInt) {
	case 0:
		//set AHeadCoveringNoHair's skeletal mesh to nothing
		AHeadCoveringNoHair->SetSkeletalMesh(nullptr);
		AHeadCoveringBaseHair->SetSkeletalMesh(nullptr);
		AHeadCoveringNoFacialHair->SetSkeletalMesh(nullptr);
		AHair->SetSkeletalMesh(AHairArray[UKismetMathLibrary::Clamp(InHero.AHairInt, 0, AHairArray.Num() - 1)]);
		break;
	case 1:
		AHeadCoveringNoHair->SetSkeletalMesh(AHeadCoverNoHairArray
			[UKismetMathLibrary::Clamp(InHero.AHeadCoverNoHairInt, 0, AHeadCoverNoHairArray.Num() - 1)]);
		AHair->SetSkeletalMesh(nullptr);
		break;
	case 2:
		AHeadCoveringBaseHair->SetSkeletalMesh(AHeadCoverBaseHairArray
			[UKismetMathLibrary::Clamp(InHero.AHeadCoverBaseHairInt, 0, AHeadCoverBaseHairArray.Num() - 1)]);
		AHair->SetSkeletalMesh(AHairArray[1]);
		break;
	case 3:
		AHeadCoveringNoFacialHair->SetSkeletalMesh(AHeadCoverNoFacialHairArray
			[UKismetMathLibrary::Clamp(InHero.AHeadCoverNoFacialHairInt, 0, AHeadCoverNoFacialHairArray.Num() - 1)]);
		AHair->SetSkeletalMesh(AHairArray[UKismetMathLibrary::Clamp(InHero.AHairInt, 0, AHairArray.Num() - 1)]);
		break;
	}
	AHeadAttachmentHelmet->SetSkeletalMesh(AHeadAttachmentHelmetArray
		[UKismetMathLibrary::Clamp(InHero.AHeadAttachmentInt, 0, AHeadAttachmentHelmetArray.Num() - 1)]);
	AChestAttachment->SetSkeletalMesh(AChestAttachmentArray
		[UKismetMathLibrary::Clamp(InHero.AChestAttachmentInt, 0, AChestAttachmentArray.Num() - 1)]);
	ABackAttachment->SetSkeletalMesh(ABackAttachmentArray
		[UKismetMathLibrary::Clamp(InHero.ABackAttachmentInt, 0, ABackAttachmentArray.Num() - 1)]);
	AShoulderAttachmentL->SetSkeletalMesh(AShoulderLAttachmentArray
		[UKismetMathLibrary::Clamp(InHero.AShoulderLAttachmentInt, 0, AShoulderLAttachmentArray.Num() - 1)]);
	AShoulderAttachmentR->SetSkeletalMesh(AShoulderRAttachmentArray
		[UKismetMathLibrary::Clamp(InHero.AShoulderRAttachmentInt, 0, AShoulderRAttachmentArray.Num() - 1)]);
	AElbowAttachmentL->SetSkeletalMesh(AElbowLAttachmentArray
		[UKismetMathLibrary::Clamp(InHero.AElbowLAttachmentInt, 0, AElbowLAttachmentArray.Num() - 1)]);
	AElbowAttachmentR->SetSkeletalMesh(AElbowRAttachmentArray
		[UKismetMathLibrary::Clamp(InHero.AElbowRAttachmentInt, 0, AElbowRAttachmentArray.Num() - 1)]);
	AHipAttachment->SetSkeletalMesh(AHipsAttachmentArray
		[UKismetMathLibrary::Clamp(InHero.AHipsAttachmentInt, 0, AHipsAttachmentArray.Num() - 1)]);
	AKneeAttachmentL->SetSkeletalMesh(AKneeLAttachmentArray
		[UKismetMathLibrary::Clamp(InHero.AKneeLAttachmentInt, 0, AKneeLAttachmentArray.Num() - 1)]);
	AKneeAttachmentR->SetSkeletalMesh(AKneeRAttachmentArray
		[UKismetMathLibrary::Clamp(InHero.AKneeRAttachmentInt, 0, AKneeRAttachmentArray.Num() - 1)]);
	AExtra->SetSkeletalMesh(AExtraArray
		[UKismetMathLibrary::Clamp(InHero.AExtraInt, 0, AExtraArray.Num() - 1)]);

	GTorso->SetMasterPoseComponent(GetMesh());
	GHead->SetMasterPoseComponent(GetMesh());
	GEyebrows->SetMasterPoseComponent(GetMesh());
	GFacialHair->SetMasterPoseComponent(GetMesh());
	GArmUpperL->SetMasterPoseComponent(GetMesh());
	GArmUpperR->SetMasterPoseComponent(GetMesh());
	GArmLowerL->SetMasterPoseComponent(GetMesh());
	GArmLowerR->SetMasterPoseComponent(GetMesh());
	GHandL->SetMasterPoseComponent(GetMesh());
	GHandR->SetMasterPoseComponent(GetMesh());
	GHips->SetMasterPoseComponent(GetMesh());
	GLegL->SetMasterPoseComponent(GetMesh());
	GLegR->SetMasterPoseComponent(GetMesh());
	AHeadCoveringNoHair->SetMasterPoseComponent(GetMesh());
	AHair->SetMasterPoseComponent(GetMesh());
	AHeadAttachmentHair->SetMasterPoseComponent(GetMesh());
	AHeadAttachmentHelmet->SetMasterPoseComponent(GetMesh());
	AChestAttachment->SetMasterPoseComponent(GetMesh());
	AHeadCoveringBaseHair->SetMasterPoseComponent(GetMesh());
	AHeadCoveringNoFacialHair->SetMasterPoseComponent(GetMesh());
	AShoulderAttachmentL->SetMasterPoseComponent(GetMesh());
	AShoulderAttachmentR->SetMasterPoseComponent(GetMesh());
	AElbowAttachmentL->SetMasterPoseComponent(GetMesh());
	AElbowAttachmentR->SetMasterPoseComponent(GetMesh());
	AHipAttachment->SetMasterPoseComponent(GetMesh());
	AKneeAttachmentL->SetMasterPoseComponent(GetMesh());
	AKneeAttachmentR->SetMasterPoseComponent(GetMesh());
	AExtra->SetMasterPoseComponent(GetMesh());
}



