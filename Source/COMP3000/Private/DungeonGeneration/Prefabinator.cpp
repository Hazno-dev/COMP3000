// Callum Stables, 2023


#include "DungeonGeneration/Prefabinator.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APrefabinator::APrefabinator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Spawned = false;
}

void APrefabinator::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
}

void APrefabinator::SpawnRandomPrefab(FRandomStream& SeedStream) {
	if (Spawned) return;
	if (PrefabMap.Num() == 0) return;
	if (SeedStream.RandRange(0.0f, 100.0f) > SpawnChance) return;

	TArray<TSubclassOf<AActor>> PrefabKeys;
	PrefabMap.GetKeys(PrefabKeys);

	bool bValidPrefabFound = false;
	TSubclassOf<AActor> SelectedPrefab;

	while (PrefabKeys.Num() > 0 && !bValidPrefabFound)
	{
		// Randomly select a prefab class and get its associated offset
		int32 RandomIndex = SeedStream.RandRange(0, PrefabKeys.Num() - 1);
		TSubclassOf<AActor> RandomPrefabClass = PrefabKeys[RandomIndex];

		// Check if the selected prefab is not too close to another one of the same class
		if (!IsPrefabNearby(RandomPrefabClass)) {
			SelectedPrefab = RandomPrefabClass;
			bValidPrefabFound = true;
		} else PrefabKeys.RemoveAt(RandomIndex);
		
	}

	if (!bValidPrefabFound)
		return;

	FTransform Offset = PrefabMap[SelectedPrefab];
	FVector SpawnLocation = GetActorLocation() + Offset.GetLocation();
	FRotator SpawnRotation = GetActorRotation() + Offset.Rotator();

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SelectedPrefab, SpawnLocation, SpawnRotation);
	Spawned = true;

	if (bRandomScale)
	{
		float RandomScale = SeedStream.RandRange(SpawnScaleMin, SpawnScaleMax);
		SpawnedActor->SetActorScale3D(FVector(RandomScale, RandomScale, RandomScale));
	}

	TArray<AActor*> AttachedActors;
	SpawnedActor->GetAllChildActors(AttachedActors);
	for (AActor* AttachedActor : AttachedActors)
	{
		if (APrefabinator* AttachedPrefabinator = Cast<APrefabinator>(AttachedActor))
		{
			AttachedPrefabinator->SpawnRandomPrefab(SeedStream);
		}
	}
}

// Called when the game starts or when spawned
void APrefabinator::BeginPlay()
{
	Super::BeginPlay();
	//SpawnRandomPrefab();
	
	
}

bool APrefabinator::IsPrefabNearby(TSubclassOf<AActor> PrefabClass) {
	if (PrefabClass == nullptr) return false;
	if (NearbyCheckRadius <= 0.0f) return false;
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PrefabClass, FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		float Distance = FVector::Dist(GetActorLocation(), FoundActor->GetActorLocation());
		if (Distance <= NearbyCheckRadius)
		{
			return true;
		}
	}
	return false;
}
