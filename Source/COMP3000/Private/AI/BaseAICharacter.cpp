// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAICharacter.h"

#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileSpawner = CreateDefaultSubobject<UProjectileSpawner>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

void ABaseAICharacter::SummonProjectile() {
	FRotator Rotator = PredictProjectileRotation();
	ProjectileSpawner->SpawnProjectile(ProjectileSpawner->GetComponentLocation(), Rotator);
}

FRotator ABaseAICharacter::PredictProjectileRotation() {
	
	AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FVector PlayerLocation = Player->GetActorLocation();
	FVector PlayerVelocity = Player->GetVelocity();

	float DistanceDir = GetDistanceTo(Player) / (ProjectileSpawner->ProjectileSpeed);
	FVector PredictedLocation = PlayerLocation + (PlayerVelocity * DistanceDir);

	FRotator Outputrotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PredictedLocation);
	return FRotator(0,Outputrotator.Yaw,0);
}



