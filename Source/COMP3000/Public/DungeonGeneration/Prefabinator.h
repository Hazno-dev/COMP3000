// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prefabinator.generated.h"

UCLASS(Blueprintable, BlueprintType, Category="Prefabs", meta=(BlueprintSpawnableComponent, DisplayName="Prefabinator", ShowCategories="Prefabs"))
class COMP3000_API APrefabinator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrefabinator();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prefabs")
	TMap<TSubclassOf<AActor>, FTransform> PrefabMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prefabs", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float SpawnChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prefabs")
	bool bRandomScale;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prefabs", meta = (EditCondition = "bRandomScale"))
	float SpawnScaleMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prefabs", meta = (EditCondition = "bRandomScale"))
	float SpawnScaleMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prefabs")
	float NearbyCheckRadius;
	
	UFUNCTION(BlueprintCallable, Category = "Prefabs")
	void SpawnRandomPrefab(FRandomStream& SeedStream);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	bool IsPrefabNearby(TSubclassOf<AActor> PrefabClass);

	bool Spawned;

	

};
