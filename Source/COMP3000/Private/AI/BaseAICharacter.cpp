// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAICharacter.h"

#include "MainCharacter.h"
#include "AI/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CapsuleComponent.h"
#include "Heroes/Abilities/AbilityData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "UI/AI/UWB_EnemyStatusBar.h"

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileSpawner = CreateDefaultSubobject<UProjectileSpawner>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	EnemyBaseAbilitiesComponent = CreateDefaultSubobject<UEnemyBaseAbilities>(TEXT("EnemyBaseAbilitiesComponent"));

	EnemyStatusEffectSystemComponent = CreateDefaultSubobject<UEnemyStatusEffectSystem>(TEXT("EnemyStatusEffectSystemComponent"));

	StatusBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusBar"));
	StatusBar->SetupAttachment(RootComponent);
	
	AIControllerClass = ABaseAIController::StaticClass();
	
	Health = 100.f;
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

	StatusBar->SetVisibility(false);

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
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCanEverAffectNavigation(false);
		PlayAnimMontage(DeathMontage, 1);
		return;
	}

	DeathDestroy();
}

void ABaseAICharacter::StopCurrentAnimation() {
	if (IsValid(GetMesh())) {
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance)) {
			AnimInstance->Montage_Stop(0.0f);
		}
	}
}
