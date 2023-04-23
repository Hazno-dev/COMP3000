// Fill out your copyright notice in the Description page of Project Settings.


#include "World/WorldCursor.h"

#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWorldCursor::AWorldCursor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CursorDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("CursorDecal"));
	CursorDecal->SetupAttachment(RootComponent);


}

void AWorldCursor::MoveCursor(FVector Location, FVector Normal) {	
	CursorDecal->SetWorldLocation(Location);
	CursorDecal->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(Normal));
}

void AWorldCursor::ScaleCursor(FVector Scale) {
	FVector OriginalScale = CursorDecal->GetComponentScale();
	CursorDecal->SetWorldScale3D(FVector(OriginalScale.X, Scale.Y, Scale.Z));
}

void AWorldCursor::ToggleVisibility(bool bVisible) {
	CursorDecal->SetVisibility(bVisible);
}

// Called when the game starts or when spawned
void AWorldCursor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldCursor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldCursor::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	CursorDecal->SetMaterial(0, NormalMaterial);
	
}

