// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OffTileArea.generated.h"

UCLASS()
class COMP3000_API AOffTileArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOffTileArea();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* OffTileAreaBox;

	UFUNCTION()
	UBoxComponent* GetBoxComponent() const { return OffTileAreaBox; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	class ALevelGeneratorV2* LevelGenerator;


	
	float Cooldown = 0.0f;
	FVector PreviousLocation;

};
