// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ProjectileBase.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	//spawn system at location
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleVFX, GetActorLocation(), GetActorRotation());
	CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit) {

 	FRotator Rotation = UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, Hit.Location, Rotation);
	this->Destroy();
}

