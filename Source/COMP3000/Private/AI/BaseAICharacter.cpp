// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAICharacter.h"

#include "MainCharacter.h"
#include "AI/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Heroes/Abilities/AbilityData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "UI/AI/UWB_EnemyStatusBar.h"
#include "World/Collectables/XPOrb.h"

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileSpawner = CreateDefaultSubobject<UProjectileSpawner>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	EnemyStatusEffectSystemComponent = CreateDefaultSubobject<UEnemyStatusEffectSystem>(TEXT("EnemyStatusEffectSystem"));
	EnemyBaseAbilitiesComponent = CreateDefaultSubobject<UEnemyBaseAbilities>(TEXT("EnemyBaseAbilitiesComponent"));

	DetectionBoxFront = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionBoxFront"));
	DetectionBoxFront->SetupAttachment(RootComponent);
	DetectionBoxFront->OnComponentBeginOverlap.AddDynamic(this, &ABaseAICharacter::MeleeDamageColliderOnOverlap);
	
	StatusBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusBar"));
	StatusBar->SetupAttachment(RootComponent);
	
	AIControllerClass = ABaseAIController::StaticClass();
	
	Health = 100.f;
	bImpulseable = false;
	XPDeathReward = 10;
}

void ABaseAICharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	
	MaxHealth = Health;
}

// Called when the game starts or when spawned
void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	ControllerRef = Cast<ABaseAIController>(GetController());

	UUWB_EnemyStatusBar* StatBarRef = Cast<UUWB_EnemyStatusBar>(StatusBar->GetUserWidgetObject());
	if (IsValid(StatBarRef)) StatBarRef->SetupComponents(this, FMath::Clamp(MaxHealth / 20.f, 3.f, 8.f));
}

// Called every frame
void ABaseAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRagdoll();
}

// Called to bind functionality to input
void ABaseAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseAICharacter::SummonProjectile(int32 NumberOfProjectiles) {
	// Set the initial spread angle for the first projectile
	float SpreadAngle = 5.0f; 

	// Loop through each projectile
	for (int32 i = 0; i < NumberOfProjectiles; ++i) {
		float CurrentAngle;
		if (i == 0) {
			CurrentAngle = 0.0f; 
		} else {
			// Alternating angles for each subsequent projectile
			CurrentAngle = ((i % 2 == 0) ? -1 : 1) * FMath::CeilToInt(i / 2.0f) * SpreadAngle;
		}

		// Calculate the rotation for the current projectile
		FRotator Rotator = PredictProjectileRotation();
		Rotator.Yaw += CurrentAngle;

		// Spawn the projectile
		ProjectileSpawner->SpawnProjectile(ProjectileSpawner->GetComponentLocation(), Rotator);
	}
}

FRotator ABaseAICharacter::PredictProjectileRotation() {
	
	AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(Player)) return FRotator(0,0,0);
	
	FVector PlayerLocation = Player->GetActorLocation();
	FVector PlayerVelocity = Player->GetVelocity();

	float DistanceDir = GetDistanceTo(Player) / (ProjectileSpawner->ProjectileSpeed);
	FVector PredictedLocation = PlayerLocation + (PlayerVelocity * DistanceDir);

	FRotator Outputrotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PredictedLocation);
	return FRotator(0,Outputrotator.Yaw,0);
}

bool ABaseAICharacter::TryMitigateDamage() {
	if (EnemyBaseAbilitiesComponent->AbilityFromType(EAbilityType::Mitigation) != nullptr) {
		ControllerRef->ReevaluateCurrentBTTask();
		EnemyBaseAbilitiesComponent->AbilityFromType(EAbilityType::Mitigation)->CoreBeginAbility();
		return true;
	}
	return false;
}

bool ABaseAICharacter::TryDeathCry() {
	if (EnemyBaseAbilitiesComponent->AbilityFromType(EAbilityType::DeathCry) != nullptr) {
		ControllerRef->ReevaluateCurrentBTTask();
		EnemyBaseAbilitiesComponent->AbilityFromType(EAbilityType::DeathCry)->CoreBeginAbility();
		return true;
	}
	return false;
}

void ABaseAICharacter::ReceivedDamage(float Damage, AActor* DamageCauser) {
	if (AIState == EAIState::Dead) return;
	
	SetTarget(DamageCauser);
	
	if (TryMitigateDamage()) return;
	
	Health -= Damage;
	TookDamageEvent.Broadcast();
	
	if (AIState == EAIState::Chase || AIState == EAIState::Idle && !GetCurrentMontage()) {
		if (IsValid(HurtMontage) && AIState != EAIState::Dead) PlayAnimMontage(HurtMontage, 1);
	}
	
	if (Health <= 0) {
		DeathStart();
	}
}

void ABaseAICharacter::DeathDestroy() {
	this->Destroy();
}

void ABaseAICharacter::MeleeDamageColliderEnabled(float Duration) {

	DetectionBoxFront->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	//Collision Disable Lambda Delegate
	FTimerHandle MeleeDamageColliderEnabledTimer;
	TWeakObjectPtr<ABaseAICharacter> WeakThis(this);
	GetWorld()->GetTimerManager().SetTimer(MeleeDamageColliderEnabledTimer, [WeakThis]() {
		if (WeakThis.IsValid()) {
			WeakThis->DetectionBoxFront->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			WeakThis->bDealtMeleeDamage = false;
		}
	}, Duration, false);
}

void ABaseAICharacter::MeleeDamageColliderOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (bDealtMeleeDamage) return;
	if (OtherActor == this) return;
	if (AMainCharacter* Player = Cast<AMainCharacter>(OtherActor)) {
		Player->TookDamage();
		bDealtMeleeDamage = true;
	}
}

void ABaseAICharacter::Ragdoll(float Duration)
{
	ControllerRef->GetAIBehaviourTreeComponent()->PauseLogic(TEXT("Ragdoll"));
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->SetSimulatePhysics(true);
	bIsRagdoll = true;

	FTimerHandle RagdollTimer;
	GetWorld()->GetTimerManager().SetTimer(RagdollTimer, this, &ABaseAICharacter::RecoverFromRagdoll, Duration, false);
}


void ABaseAICharacter::UpdateRagdoll() {
	if (bIsRagdoll) {
		// Get the location of the mesh's root bone or pelvis
		FName RootBoneName = GetMesh()->GetBoneName(0); // This should be the pelvis or root bone
		FVector MeshRootLocation = GetMesh()->GetSocketLocation(RootBoneName);
		GetCapsuleComponent()->SetWorldLocation(MeshRootLocation + FVector(0, 0, 90));
	}
}

void ABaseAICharacter::RecoverFromRagdoll()
{
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetAllBodiesSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bIsRagdoll = false;
	
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	ControllerRef->GetAIBehaviourTreeComponent()->ResumeLogic(TEXT("Ragdoll Ended"));
}




void ABaseAICharacter::SetTarget(AActor* NewTarget) {
	if (AIState == EAIState::Dead) return;
	if (!IsValid(NewTarget) || !IsValid(ControllerRef)) return;
	
	if (AMainCharacter* CharacterTarget = Cast<AMainCharacter>(NewTarget))
		ControllerRef->GetBlackboardComponent()->SetValueAsObject("ActorTarget", CharacterTarget);

	ControllerRef->GetAIPerceptionComponent()->SetActive(false);
	StatusBar->SetVisibility(true);
	DetectedEvent.Broadcast();
}

void ABaseAICharacter::DeathStart() {
	AIState = EAIState::Dead;
	EventChangedAIState.Broadcast();

	StatusBar->SetVisibility(false);

	AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(Player)) {
		if (IsValid(XPOrbClass)) DeathDrops();
		else Player->GainXP(XPDeathReward);
	}

	if (TryDeathCry()) return;
	
	if (!IsValid(ControllerRef)) {
		DeathDestroy();
		return;
	}
	
	if (IsValid(DeathMontage)) {
		StopAnimMontage();
		StopCurrentAnimation();
		
		ControllerRef->StopMovement();
		if (IsValid(ControllerRef->GetAIBehaviourTreeComponent())) {
			ControllerRef->GetAIBehaviourTreeComponent()->StopTree();
			ControllerRef->GetAIBehaviourTreeComponent()->StopLogic("Death");
		}
		
		if (IsValid(ControllerRef->GetAIPerceptionComponent())) {
			ControllerRef->GetAIPerceptionComponent()->SetSenseEnabled(UAISense::StaticClass(), false);
		}
		ControllerRef->ClearFocus(EAIFocusPriority::Gameplay);
		GetCharacterMovement()->bUseRVOAvoidance = false;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCanEverAffectNavigation(false);
		PlayAnimMontage(DeathMontage, 1);
		return;
	}

	DeathDestroy();
}

void ABaseAICharacter::DeathDrops() {
	const float Angle = 50;
	const float Strength = 150;

	int OrbCount = 1;
	if (XPDeathReward <= 0) return;
	if (XPDeathReward > 30) OrbCount = 2;
	if (XPDeathReward > 100) OrbCount = 3;

	const float Reward = XPDeathReward / OrbCount;
	
	for (int i = 0; i < OrbCount; i++) {
		AXPOrb* XPOrb = GetWorld()->SpawnActor<AXPOrb>(XPOrbClass, GetActorLocation() + FVector(0, 0, 50), FRotator(0, 0, 0));
		if (!IsValid(XPOrb)) continue;
		XPOrb->LaunchOrb(Strength, Angle);
		XPOrb->SetXPValue(FMath::CeilToInt32( Reward));
	}
}

void ABaseAICharacter::StopCurrentAnimation() {
	if (IsValid(GetMesh())) {
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance)) {
			AnimInstance->Montage_Stop(0.0f);
		}
	}
}
