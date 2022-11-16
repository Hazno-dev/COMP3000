// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class COMP3000_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;
	
public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Is character holding punch
	UPROPERTY(BlueprintReadOnly, Category = "PlayerController")
	bool Punching = false;

protected:

	/** Forward/Backwards motion */
	void MoveForward(float Value);

	/** Left/Right motion */
	void MoveRight(float Value);

	/** Start punching */
	void Punch();

	/** Stop punching */
	void StopPunch();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	//FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	AController* SavedController;

};
