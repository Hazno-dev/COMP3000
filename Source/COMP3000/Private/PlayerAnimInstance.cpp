// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"


#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	Velocity = FVector::ZeroVector;
	GroundSpeed = 0;
	Direction = 0;
	ShouldMove = false;
	IsFalling = false;
	IsPunching = false;
	LastRot = FRotator::ZeroRotator;
	CurrentRot = FRotator::ZeroRotator;
	YawRootOffset = 0;
	DistanceCurve = 0;
	MaxTurnAngle = 90;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwningCharacter = Cast<AMainCharacter>(GetOwningActor());
	if (OwningCharacter != nullptr) {
		OwningMovement = OwningCharacter->GetCharacterMovement();
		OwningCharacter->ArmedToggle.AddDynamic(this, &UPlayerAnimInstance::ArmedToggleMontage);
		OwningCharacter->FistFire.AddDynamic(this, &UPlayerAnimInstance::FistFireMontage);
	}
}


void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (OwningMovement != nullptr)
	{
		//Set values for animation state machine
		Velocity = OwningMovement->Velocity;
		GroundSpeed = Velocity.Size();
		ShouldMove = (GroundSpeed > 0 && OwningMovement->GetCurrentAcceleration() != FVector::ZeroVector) ? true : false;
		IsFalling = OwningMovement->IsFalling();
		Direction = CalculateDirection(Velocity, OwningCharacter->PublicRot);
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::SanitizeFloat(Direction));
		IsPunching = OwningCharacter->Punching;

		//Switch between montages for upper torso, dependant on action
		MontagePlayer();

		//Turn in place leg system
		TurnInPlace();
	}
}

//Play upper-torso montages
void UPlayerAnimInstance::MontagePlayer()
{
	//Punching start/stop
	//if (IsPunching && !this->Montage_IsPlaying(OwningCharacter->Mon_Punching)) this->Montage_Play(OwningCharacter->Mon_Punching, 1);
	//else if (!IsPunching && this->Montage_IsPlaying(OwningCharacter->Mon_Punching)) this->Montage_Stop(0.2, OwningCharacter->Mon_Punching);

	//Walking start
	//if (!IsPunching && ShouldMove && !this->Montage_IsPlaying(OwningCharacter->Mon_Walking)) this->Montage_Play(OwningCharacter->Mon_Walking, 1);
	//else if (!IsPunching && !ShouldMove && this->Montage_IsPlaying(OwningCharacter->Mon_Walking)) this->Montage_Stop(0.2, OwningCharacter->Mon_Walking);
}

//Handle turning in-place animations for player standing still
void UPlayerAnimInstance::TurnInPlace()
{
	OwningCharacter->GetMesh()->bEnableUpdateRateOptimizations = false;
	OwningCharacter->GetMesh()->bUpdateJointsFromAnimation = true;
	OwningCharacter->GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	OwningCharacter->GetMesh()->SetAllowAnimCurveEvaluation(true);

	LastRot = CurrentRot;
	CurrentRot = OwningCharacter->PublicRot;
	YawChangeOF = UKismetMathLibrary::NormalizedDeltaRotator(CurrentRot, LastRot).Yaw;
	if (ShouldMove || IsFalling) {YawRootOffset = 0; return;}
	YawRootOffset = UKismetMathLibrary::NormalizeAxis(YawRootOffset - YawChangeOF);
	if (OwningMovement->Velocity.Size() == 0 && OwningMovement->GetCurrentAcceleration() == FVector::ZeroVector) OwningCharacter->SetActorRotation(CurrentRot);
	Turning = GetCurveValue("Turning");
	if (Turning > 0)
	{
		DistanceCurveLF = DistanceCurve;
		DistanceCurve = GetCurveValue("DistanceCurve");
		//Set direction to L or R
		DistanceCurveDiff = DistanceCurve - DistanceCurveLF;
		YawRootOffset = (YawRootOffset > 0) ? YawRootOffset - DistanceCurveDiff : YawRootOffset + DistanceCurveDiff;
		AbsoluteYawRoofOffset = UKismetMathLibrary::Abs(YawRootOffset);
		if (AbsoluteYawRoofOffset > MaxTurnAngle)
		{
			const float Excess = AbsoluteYawRoofOffset - MaxTurnAngle;
			YawRootOffset = (YawRootOffset > 0) ? YawRootOffset - Excess : YawRootOffset + Excess;
		}
		if (DistanceCurve == 0 && AbsoluteYawRoofOffset >= 90) YawRootOffset = (YawRootOffset > 0) ? YawRootOffset - MaxTurnAngle : YawRootOffset + MaxTurnAngle;
		
	} 
	
}

void UPlayerAnimInstance::ArmedToggleMontage() {
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::SanitizeFloat(OwningCharacter->Armed));
	if (OwningCharacter->Armed && !this->Montage_IsPlaying(OwningCharacter->Mon_EnterArmed)) this->Montage_Play(OwningCharacter->Mon_EnterArmed, 1);
	else if (!OwningCharacter->Armed && !this->Montage_IsPlaying(OwningCharacter->Mon_ExitArmed)) this->Montage_Play(OwningCharacter->Mon_ExitArmed, 1);
}

void UPlayerAnimInstance::FistFireMontage() {
	if (!OwningCharacter->Punching) return;
	OwningCharacter->CanFire = false;

	//gengine firinghand
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::SanitizeFloat(FiringHand));

	if (!FiringHand) {
		this->Montage_Play(OwningCharacter->Mon_FireFistRight, 1);
		FiringHand = true;
		OwningCharacter->SetHandParticlesOnL(true);
	}
	else {
		this->Montage_Play(OwningCharacter->Mon_FireFistLeft, 1);
		FiringHand = false;
		OwningCharacter->SetHandParticlesOnR(true);
	}
}




