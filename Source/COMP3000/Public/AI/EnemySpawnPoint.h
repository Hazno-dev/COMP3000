// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyData.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnPoint.generated.h"

class ABaseAICharacter;
UCLASS()
class COMP3000_API AEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnPoint();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Point Data")
	TEnumAsByte<EEnemyType> EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Point Data")
	bool bTryFindNearestGroupManager;

	UFUNCTION()
	void SpawnEnemy(FEnemyData& EnemyData);

	UPROPERTY()
	bool bHasSpawned;

	UPROPERTY()
	bool bIsRegistered;

	UPROPERTY()
	bool bSecondPass;

	UPROPERTY()
	class ATileBase* AssociatedTB;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
