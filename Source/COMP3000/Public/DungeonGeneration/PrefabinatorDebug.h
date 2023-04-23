// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrefabinatorDebug.generated.h"

UCLASS()
class COMP3000_API APrefabinatorDebug : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrefabinatorDebug();

	virtual void Tick(float DeltaTime) override;

	void SpawnAllPrefabs();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prefabs")
	bool bSpawnAllPrefabs;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FRandomStream SeedStream;
};
