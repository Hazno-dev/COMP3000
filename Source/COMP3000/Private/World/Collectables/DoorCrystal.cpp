// Callum Stables, 2023


#include "World/Collectables/DoorCrystal.h"

#include "MainCharacter.h"

// Sets default values
ADoorCrystal::ADoorCrystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CrystalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrystalMesh"));
	CrystalMesh->SetupAttachment(RootComponent);

	CrystalCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CrystalCollision"));
	CrystalCollision->SetupAttachment(CrystalMesh);
}

// Called when the game starts or when spawned
void ADoorCrystal::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(CrystalCollision)) CrystalCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoorCrystal::OnOverlapBegin);
}

void ADoorCrystal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (!IsValid(OtherActor)) return;

	// Destroy self if the player overlaps and they don't have the key
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor)) {
		if (MainCharacter->bHasKeyCrystal) return;
		
		if (IsValid(NiagaraFX) && IsValid(GetWorld())) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraFX, GetActorLocation());
		if (IsValid(NiagaraFX2) && IsValid(GetWorld())) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraFX2, GetActorLocation());
		MainCharacter->SetHasKeyCrystal(true);
		Destroy();
	}
}
