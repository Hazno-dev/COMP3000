// Callum Stables, 2023


#include "World/PSpawnPoint.h"

#include "MainCharacter.h"

// Sets default values
APSpawnPoint::APSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	EnterSpawnPointRange = CreateDefaultSubobject<UBoxComponent>(TEXT("EnterSpawnPointRange"));
	EnterSpawnPointRange->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	EnterSpawnPointRange->OnComponentBeginOverlap.AddDynamic(this, &APSpawnPoint::OnEnterSpawnPointRangeBeginOverlap);

}

void APSpawnPoint::OnEnterSpawnPointRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (AMainCharacter* Player = Cast<AMainCharacter>(OtherActor))
	{
		Player->SetSpawnPoint(this);
	}
}

