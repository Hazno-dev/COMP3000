// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "MainCharacter.generated.h"


UENUM(BlueprintType)
enum class E_PlayerDirection : uint8 {
	NONE UMETA(DisplayName = "NONE"),
	LEFT UMETA(DisplayName = "LEFT"),
	RIGHT UMETA(DisplayName = "RIGHT")
};
UCLASS()
class COMP3000_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	//UNREAL DEFAULTS
	// Sets default values for this character's properties
	AMainCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind inputs
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	//COMPONENT DEFINITIONS
	/** Main Camera */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;
	
	/** Camera springarm */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

	/** Player Controller reference */
	UPROPERTY()
	APlayerController* SavedController;

	
	//PUBLIC PLAYER VARIABLES
	/** Character holding punch */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerController")
	bool Punching = false;

	/** Players rotation  */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerController")
	FRotator PublicRot = FRotator(0,0,0);

	/** Players Rotation Direction */
	UPROPERTY(EditAnywhere, Category = "PlayerController")
	TEnumAsByte<E_PlayerDirection> PlayerDirection;

	//PUBLIC PLAYER STATS
	/** Players Health */
	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	int PlayerHealth;
	
	//PLAYER MONTAGES
	/** Punching Montage  */
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* Mon_Punching;

	/** Punching Montage  */
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* Mon_Walking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionStimuliSourceComponent> AIStimuliSource;
	
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

private:

	/** Trace Channel for Mouse Aiming */
	ETraceTypeQuery TraceChannel;
	
	/** Hitpoint for Mouse Aiming */
	FHitResult HitResult;

};
