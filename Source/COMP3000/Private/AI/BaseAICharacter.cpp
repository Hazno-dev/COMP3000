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

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileSpawner = CreateDefaultSubobject<UProjectileSpawner>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	EnemyBaseAbilitiesComponent = CreateDefaultSubobject<UEnemyBaseAbilities>(TEXT("EnemyBaseAbilitiesComponent"));

	AIControllerClass = ABaseAIController::StaticClass();
	
	Health = 100.f;
}

// Called when the game starts or when spawned
void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	ControllerRef = Cast<ABaseAIController>(GetController());
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

void ABaseAICharacter::ReceivedDamage(float Damage, AActor* DamageCauser) {
	SetTarget(DamageCauser);
	TookDamageEvent.Broadcast();
	
	if (TryMitigateDamage()) return;
	
	Health -= Damage;
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
}

void ABaseAICharacter::DeathStart() {
	AIState = EAIState::Dead;
	
	if (IsValid(DeathMontage)) {
		StopAnimMontage();
		StopCurrentAnimation();
		if (IsValid(ControllerRef)) {
			ControllerRef->StopMovement();
			if (IsValid(ControllerRef->GetAIBehaviourTreeComponent())) {
				ControllerRef->GetAIBehaviourTreeComponent()->StopTree();
				ControllerRef->GetAIBehaviourTreeComponent()->StopLogic("Death");
			}
		}
		if (IsValid(ControllerRef->GetAIPerceptionComponent())) {
			ControllerRef->GetAIPerceptionComponent()->SetSenseEnabled(UAISense::StaticClass(), false);
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlayAnimMontage(DeathMontage, 1);
		//FTimerHandle DeathTimerHandle;
		//GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &ABaseAICharacter::DeathDestroy, 1, false);

	}
	else DeathDestroy();
}

void ABaseAICharacter::StopCurrentAnimation() {
	if (IsValid(GetMesh())) {
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance)) {
			AnimInstance->Montage_Stop(0.0f);
		}
	}
}
