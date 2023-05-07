// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/InterpToMovementComponent.h"
#include "Components/TimelineComponent.h"

#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "LockedDoor.generated.h"

UENUM()
enum DoorSide
{
	Front,
	Back
};

UCLASS()
class COMP3000_API ALockedDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALockedDoor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> DoorMesh1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> DoorMesh2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> DoorCollisionFront;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> DoorCollisionBack;

	UPROPERTY()
	UTimelineComponent* DoorTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	UCurveFloat* DoorCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Data")
	bool bKeyDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Data")
	bool bIsOpenDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Data")
	bool bWaveDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Data")
	bool bIsOneShot;

	UFUNCTION()
	void ControlDoor(float Value);

	UFUNCTION()
	void CloseDoor();

	UFUNCTION()
	void DoorFinishedClosing();

	UFUNCTION()
	void OpenDoor();

	UFUNCTION()
	void DoorFinishedOpening();
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* DoorOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* DoorOpenAttenuation;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool bIsClosed;
	bool bIsClosing;

	FVector DoorStartLocation;
	

};
