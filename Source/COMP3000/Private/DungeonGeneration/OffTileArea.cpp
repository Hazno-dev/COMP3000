// Callum Stables, 2023


#include "DungeonGeneration/OffTileArea.h"

#include "Components/BoxComponent.h"
#include "DungeonGeneration/LevelGeneratorV2.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOffTileArea::AOffTileArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OffTileAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OffTileAreaBox"));
	RootComponent = OffTileAreaBox;
}

void AOffTileArea::BeginPlay() {
	Super::BeginPlay();

	LevelGenerator = Cast<ALevelGeneratorV2>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelGeneratorV2::StaticClass()));
	PreviousLocation = GetActorLocation();

}

void AOffTileArea::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!IsValid(LevelGenerator)) return;

	Cooldown++;
	if (GetActorLocation() != PreviousLocation && Cooldown > 2) {
		Cooldown = 0;
		PreviousLocation = GetActorLocation();
		LevelGenerator->ResetAndRegenerate();
	}
}
