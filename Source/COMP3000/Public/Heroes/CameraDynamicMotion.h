// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CameraDynamicMotion.generated.h"


class AMainCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMP3000_API UCameraDynamicMotion : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraDynamicMotion();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Data")
	float CameraLookMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Data")
	float CameraZoomMaxFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Data")
	float MaxSpringArmLength;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCameraLag(float Lag) const { SpringArmRef->CameraLagSpeed = Lag; };

private:

	UPROPERTY()
	AMainCharacter* MainCharacterRef;

	UPROPERTY()
	UCameraComponent* CameraRef;

	UPROPERTY()
	USpringArmComponent* SpringArmRef;

	UPROPERTY()
	FVector OffsetLocation;

	float CameraZoomMinFOV;

	float CameraZoomTargetFOV;

	float CameraZoomSpeed;

	float MinSpringArmLength;

	float DefaultCameraLag;

	void CameraOffset(float DeltaTime);

	bool CameraConstraints(float NewPitch);
	
	void MovementFOV(float DeltaTime);

	void MovementZoom(float DeltaTime);

	void CameraLagDecay(float DeltaTime);
	



};



