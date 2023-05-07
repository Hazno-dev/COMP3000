// Callum Stables, 2023


#include "AI/Abilities/EAB_Charge.h"

#include "MainCharacter.h"
#include "AI/BaseAICharacter.h"
#include "AI/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AEAB_Charge::BeginAbility() {
	Super::BeginAbility();

	if (ChargeSpeed == 0 || !IsValid(StunnedStatusEffect->GetDefaultObject())) return;

	AIControllerRef = Cast<ABaseAIController>(AICharacterRef->GetController());
	if (!IsValid(AIControllerRef)) return;

	TargetActor = AIControllerRef->GetFocusActor();

	if (!IsValid(TargetActor)) return;

	AMainCharacter* MainCharacterRef = Cast<AMainCharacter>(TargetActor);
	if (!IsValid(MainCharacterRef)) return;
	
	NormalMoveSpeed = AICharacterRef->GetCharacterMovement()->MaxWalkSpeed;
	AICharacterRef->GetCharacterMovement()->MaxWalkSpeed = ChargeSpeed;

	AICharacterRef->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEAB_Charge::OnHit);

	ChargeDirection = (MainCharacterRef->GetActorLocation() - AICharacterRef->GetActorLocation()).GetSafeNormal();

	AIControllerRef->ClearFocus(EAIFocusPriority::Gameplay);

	AICharacterRef->GetCharacterMovement()->bUseRVOAvoidance = false;
	AICharacterRef->GetCharacterMovement()->AvoidanceConsiderationRadius = 0.0f;

	if (ChargeStartSound && ChargeStartAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ChargeStartSound, AICharacterRef->GetActorLocation(), FRotator::ZeroRotator, .5f, 1.0f, 0.0f, ChargeStartAttenuation);
	else if (ChargeStartSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ChargeStartSound, AICharacterRef->GetActorLocation(), FRotator::ZeroRotator, .5f, 1.0f);
	
}

void AEAB_Charge::PlayingAbility() {
	Super::PlayingAbility();

	if (!IsValid(AICharacterRef)) return;

	AICharacterRef->AddMovementInput(ChargeDirection, 1.0f);

	FootstepCounter -= WorldRef->GetDeltaSeconds();
	if (FootstepCounter <= 0.0f) {
		FootstepCounter = 0.2f;
		if (ChargeFootsteps.Num() > 0 && ChargeFootstepsAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ChargeFootsteps[FMath::RandRange(0, ChargeFootsteps.Num() - 1)], AICharacterRef->GetActorLocation(), FRotator::ZeroRotator, .5f, 1.0f, 0.0f, ChargeFootstepsAttenuation);
		else if (ChargeFootsteps.Num() > 0) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ChargeFootsteps[FMath::RandRange(0, ChargeFootsteps.Num() - 1)], AICharacterRef->GetActorLocation(), FRotator::ZeroRotator, .5f, 1.0f);
	}
}

void AEAB_Charge::EndAbility() {

	ChargeEnd();
	
	Super::EndAbility();
}

void AEAB_Charge::InterruptAbility() {

	ChargeEnd();
	
	Super::InterruptAbility();
}

void AEAB_Charge::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit) {

	if (!bIsAbilityPlaying) return;

	float AngleBetweenNormals = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Hit.ImpactNormal, FVector(0, 0, 1))));
	if (AngleBetweenNormals < 25.0f) {
		return; 
	}

	if (ABaseAICharacter* HitAICharacter = Cast<ABaseAICharacter>(OtherActor)) {
		// Check if the AICharacter can be impulsed
		if (HitAICharacter->bImpulseable) {

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hit AI Character"));
			
			// Calculate the impulse direction
			FVector ChargeDirection2D = FVector(ChargeDirection.X, ChargeDirection.Y, 0.0f).GetSafeNormal();
			FVector ImpactNormal2D = FVector(Hit.ImpactNormal.X, Hit.ImpactNormal.Y, 0.0f).GetSafeNormal();
			FVector CrossProduct = FVector::CrossProduct(ChargeDirection2D, ImpactNormal2D);
			FVector ImpulseDirection = FVector::CrossProduct(CrossProduct, ImpactNormal2D).GetSafeNormal();

			// yeet enemies
			ImpulseDirection.Z = 0.5f;
			ImpulseDirection.Normalize();

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Impulse Direction: ") + ImpulseDirection.ToString());
			
			// Apply the impulse
			float ImpulseStrength = 500.0f; // Adjust this value as needed

			FVector ClampedImpulse = ImpulseDirection * ImpulseStrength;
			ClampedImpulse = ClampedImpulse.GetClampedToMaxSize(200.f);
			//HitAICharacter->GetCharacterMovement()->AddImpulse(ImpulseDirection * ImpulseStrength, true);
			
			USkeletalMeshComponent* EnemySkeletalMesh = HitAICharacter->GetMesh();
			if (EnemySkeletalMesh)
			{
				//EnemySkeletalMesh->AddRadialImpulse(AICharacterRef->GetActorLocation(), 10.f, 1000.f, ERadialImpulseFalloff::RIF_Linear, true);
				HitAICharacter->Ragdoll(2);
				EnemySkeletalMesh->AddRadialImpulse(AICharacterRef->GetActorLocation(), 400.f, 1000.f, ERadialImpulseFalloff::RIF_Constant, true);
			}
			
			return;
		}
	}
	
	float DotProduct = FVector::DotProduct(Hit.ImpactNormal, FVector(0, 0, 1));
	float Threshold = 0.9;
	if (DotProduct > Threshold) {
		return; // Do not interrupt the ability if it's a floor collision
	}

	CoreInterruptAbility();

	if (AMainCharacter* MainCharacterRef = Cast<AMainCharacter>(OtherActor)) {
		if (!bHitPlayer) {
			MainCharacterRef->TookDamage();
			bHitPlayer = true;
		}
	}
}

void AEAB_Charge::ChargeEnd() {
	AICharacterRef->GetCharacterMovement()->bUseRVOAvoidance = bOriginalRVOEnabled;
	AICharacterRef->GetCharacterMovement()->AvoidanceConsiderationRadius = OriginalAvoidanceWeight;

	AICharacterRef->GetCharacterMovement()->MaxWalkSpeed = NormalMoveSpeed;

	AICharacterRef->EnemyStatusEffectSystemComponent->AddStatusEffect(StunnedStatusEffect, 2.f, 1.f);
	
	AICharacterRef->GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &AEAB_Charge::OnHit);

	AIControllerRef->SetFocus(TargetActor, EAIFocusPriority::Gameplay);
	if (IsValid(AIControllerRef) && IsValid(TargetActor)) AIControllerRef->GetBlackboardComponent()->SetValueAsVector("TargetLocation", TargetActor->GetActorLocation());
}

