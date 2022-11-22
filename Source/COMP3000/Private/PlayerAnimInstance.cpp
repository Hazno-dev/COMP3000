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
	LastYaw = 0;
	CurrentYaw = 0;
	YawRootOffset = 0;
	DistanceCurve = 0;
	MaxTurnAngle = 90;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwningCharacter = Cast<AMainCharacter>(GetOwningActor());
	if (OwningCharacter != nullptr) OwningMovement = OwningCharacter->GetCharacterMovement();
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
	if (IsPunching && !this->Montage_IsPlaying(OwningCharacter->Mon_Punching)) this->Montage_Play(OwningCharacter->Mon_Punching, 1);
	else if (!IsPunching && this->Montage_IsPlaying(OwningCharacter->Mon_Punching)) this->Montage_Stop(0.2, OwningCharacter->Mon_Punching);

	//Walking start
	if (!IsPunching && ShouldMove && !this->Montage_IsPlaying(OwningCharacter->Mon_Walking)) this->Montage_Play(OwningCharacter->Mon_Walking, 1);
	else if (!IsPunching && !ShouldMove && this->Montage_IsPlaying(OwningCharacter->Mon_Walking)) this->Montage_Stop(0.2, OwningCharacter->Mon_Walking);
}

//Handle turning in-place animations for player standing still
void UPlayerAnimInstance::TurnInPlace()
{
	//if (Turning > 0 && DistanceCurve >= 90)
	//{
	//	OwningCharacter->SetActorRotation(FRotator(0, OwningCharacter->PublicRot.Yaw, 0));
	//	if (YawRootOffset > MaxTurnAngle) YawRootOffset = YawRootOffset - MaxTurnAngle;
	//	else if (YawRootOffset < MaxTurnAngle *-1) YawRootOffset = YawRootOffset + MaxTurnAngle;
	//}
	OwningCharacter->GetMesh()->bEnableUpdateRateOptimizations = false;
	OwningCharacter->GetMesh()->bUpdateJointsFromAnimation = true;
	OwningCharacter->GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	OwningCharacter->GetMesh()->SetAllowAnimCurveEvaluation(true);

	LastYaw = CurrentYaw;
	CurrentYaw = OwningCharacter->PublicRot.Yaw;
	YawChangeOF = LastYaw - CurrentYaw;
	if (ShouldMove) {YawRootOffset = 0; return;}
	YawRootOffset = UKismetMathLibrary::NormalizeAxis(YawChangeOF + YawRootOffset);
	Turning = GetCurveValue("Turning");
	GEngine->AddOnScreenDebugMessage( -1,1.0,FColor::Black, FString::SanitizeFloat(YawRootOffset) );
	
	if (Turning > 0)
	{
		//OwningCharacter->IsTurning = true;
		DistanceCurveLF = DistanceCurve;
		DistanceCurve = GetCurveValue("DistanceCurve");
		//Set direction to L or R
		LeftOrRight = 1 - 2*(YawRootOffset > 0);
		DistanceCurveDiff = DistanceCurveLF - DistanceCurve;
		YawRootOffset = (DistanceCurveDiff * LeftOrRight) - YawRootOffset;
		AbsoluteYawRoofOffset = UKismetMathLibrary::Abs(YawRootOffset);
		if (AbsoluteYawRoofOffset > MaxTurnAngle)
		{
			YawRootOffset = (YawRootOffset > 0) ? (AbsoluteYawRoofOffset - MaxTurnAngle) * -1 : (AbsoluteYawRoofOffset - MaxTurnAngle) * 1;
			//YawRootOffset = YawRootOffset - YawToSub;
			//if (YawRootOffset > MaxTurnAngle) YawRootOffset = YawRootOffset - MaxTurnAngle;
			//else if (YawRootOffset < MaxTurnAngle *-1) YawRootOffset = YawRootOffset + MaxTurnAngle;
		}
		//GEngine->AddOnScreenDebugMessage( -1,1.0,FColor::Red, FString::SanitizeFloat(AbsoluteYawRoofOffset) );
	}
	
}




