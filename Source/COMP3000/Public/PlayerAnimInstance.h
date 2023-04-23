// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class AMainCharacter;
/**
 * 
 */
UCLASS(Transient, Blueprintable, HideCategories=AnimInstance, BlueprintType)
class COMP3000_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnimInstance();

	//REFERENCE VARIABLES
	/** Owning character of this animinstance */
	UPROPERTY(BlueprintReadOnly, Category = "Reference")
	AMainCharacter* OwningCharacter;

	/** Owning characters movement component */
	UPROPERTY(BlueprintReadOnly, Category = "Reference")
	UCharacterMovementComponent* OwningMovement;

	//PLAYER DATA
	/** Players Velocity  */
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Data")
	FVector Velocity;

	/** Players ground speed as a float  */
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Data")
	float GroundSpeed;

	/** Players rotation  */
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Data")
	float Direction;
	
	/** Is player moving  */
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Data")
	bool ShouldMove;

	/** Is player falling  */
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Data")
	bool IsFalling;

	/** Is player punching  */
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Data")
	bool IsPunching;

	/** Is Casting */
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Data")
	bool IsCasting;

	//TURN IN PLACE DATA
	/** Players net Yaw offset  */
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Data")
	float YawRootOffset;

	/** Players net Yaw offset  */
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Data")
	float DistanceCurve;

	/** Players net Yaw offset  */
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement Data")
	float MaxTurnAngle;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeInitializeAnimation() override;

	void MontagePlayer();

	void TurnInPlace();

	UFUNCTION()
	void ArmedToggleMontage();
	
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void FistFireMontage();

	UFUNCTION()
	void StartCast();

	UFUNCTION()
	void EndCastMontage();

	UFUNCTION()
	void CancelledCastMontage();

private:

	//TURN IN PLACE PRIVATE DATA
	/** Players turning  */
	UPROPERTY() float Turning;
	/** Players net Yaw offset  */
	UPROPERTY() float DistanceCurveLF;
	/** Players net Yaw offset  */
	UPROPERTY() float DistanceCurveDiff;
	/** Players net Yaw offset  */
	UPROPERTY() float AbsoluteYawRoofOffset;
	/** Players last Yaw rot  */
	UPROPERTY() FRotator LastRot;
	/** Players current Yaw rot  */
	UPROPERTY() FRotator CurrentRot;
	/** Players Yaw change/frame  */
	UPROPERTY() float YawChangeOF;

	bool FiringHand = false;
	
};
