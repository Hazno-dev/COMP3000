// Callum Stables, 2023


#include "World/Collectables/XPOrb.h"

#include "MainCharacter.h"

// Sets default values
AXPOrb::AXPOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InnerSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereCollider"));
	InnerSphereCollider->SetupAttachment(SphereMesh);
	InnerSphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AXPOrb::OnOverlapBegin);

	OuterSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereCollider"));
	OuterSphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AXPOrb::OnOverlapBegin);
	OuterSphereCollider->SetupAttachment(SphereMesh);

	bOuterBoost = false;
	XPValue = 0;
	Player = nullptr;
	MinimumLifeTime = 0.0f;
}


// Called when the game starts or when spawned
void AXPOrb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AXPOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsValid(Player)) MoveTowardsPlayer(25.0f);

	if (MinimumLifeTime <= .3f) {
		MinimumLifeTime += DeltaTime;
		if (MinimumLifeTime >= .3f) RecheckInner();
	}


}

void AXPOrb::LaunchOrb(float LaunchStrength, float LaunchAngle) {
	
	SphereMesh->SetSimulatePhysics(true);

	// random direction
	float RandomAngle = FMath::RandRange(0.0f, 360.0f);
	FVector LaunchDirection = FVector(FMath::Cos(FMath::DegreesToRadians(RandomAngle)), FMath::Sin(FMath::DegreesToRadians(RandomAngle)), 0.0f);


	// Calculate launch velocity based on launch direction and launch strength
	FVector LaunchVelocity = LaunchStrength * LaunchDirection;

	// Calculate Z velocity based on launch angle
	float ZVelocity = LaunchStrength * FMath::Tan(FMath::DegreesToRadians(LaunchAngle));
	LaunchVelocity.Z = ZVelocity;

	SphereMesh->SetPhysicsLinearVelocity(LaunchVelocity);
}


void AXPOrb::MoveTowardsPlayer(float InForce) {
	const FVector Direction = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	const FVector Force = Direction * InForce;
	SphereMesh->SetPhysicsLinearVelocity(Force, true);
}

void AXPOrb::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	Player = Cast<AMainCharacter>(OtherActor);
	if (!IsValid(Player)) return;

	if (OverlappedComp == OuterSphereCollider && !bOuterBoost) {
		LaunchOrb(8.0f, 89.0f);
		bOuterBoost = true;
	}

	if (MinimumLifeTime < .3f) return;
	
	if (OverlappedComp == InnerSphereCollider) {
		if (XPValue > 0) Player->GainXP(XPValue);
		Destroy();
	}
}

void AXPOrb::RecheckInner() {
	TArray<AActor*> OverlappingActors;
	InnerSphereCollider->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors) {
		if (AMainCharacter* PlayerLocal = Cast<AMainCharacter>(Actor)) {
			if (XPValue > 0) Player->GainXP(XPValue);
			Destroy();
		}
	}
}
