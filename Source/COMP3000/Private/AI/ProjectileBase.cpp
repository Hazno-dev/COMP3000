// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ProjectileBase.h"

#include "MainCharacter.h"
#include "AI/BaseAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "World/Collectables/LootCrate.h"

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
	if (IsValid(MuzzleVFX)) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleVFX, GetActorLocation(), GetActorRotation());
	CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::LifeSpanExpired() {


	OnProjectileHit.Broadcast();
	Destroy();
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit) {

 	FRotator Rotation = UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal);
	if (IsValid(HitVFX)) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, Hit.Location, Rotation);

	if (ImpactSound && ImpactAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location, FRotator::ZeroRotator, 1.f, 1.f, 0.f, ImpactAttenuation);
	else if (ImpactSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location, FRotator::ZeroRotator, 1.f, 1.f, 0.f);

	OnProjectileHit.Broadcast();
	
	this->Destroy();
}

void AProjectileBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	FRotator Rotation = UKismetMathLibrary::MakeRotFromX(SweepResult.ImpactNormal);
	if (IsValid(HitVFX)) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, SweepResult.Location, Rotation);


	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Damage: %f"), Damage));
	
	if (Damage > 0) {

		if (ABaseAICharacter* AI = Cast<ABaseAICharacter>(OtherActor)) {
			AI->ReceivedDamage(Damage, this->GetOwner());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit AI")));
		}
	}

	if (AMainCharacter* MainCharacterRef = Cast<AMainCharacter>(OtherActor)) {
		MainCharacterRef->TookDamage();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit Player")));
	}

	if (ALootCrate* LootCrate = Cast<ALootCrate>(OtherActor)) {
		LootCrate->OpenLootCrate();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit LootCrate")));
	}

	
	if (ImpactSound && ImpactAttenuation) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, SweepResult.Location, FRotator::ZeroRotator, .4f, 1.f, 0.f, ImpactAttenuation);
	else if (ImpactSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, SweepResult.Location, FRotator::ZeroRotator, .4f, 1.f, 0.f);

	OnProjectileHit.Broadcast();
	
	this->Destroy();
}


