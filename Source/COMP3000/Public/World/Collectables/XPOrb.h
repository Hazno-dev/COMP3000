// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "XPOrb.generated.h"

UCLASS()
class COMP3000_API AXPOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXPOrb();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> SphereMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USphereComponent> InnerSphereCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USphereComponent> OuterSphereCollider;

	//Arc Functions
	//Launch Orb towards target pos
	UFUNCTION()
	void LaunchOrb(float LaunchStrength, float LaunchAngle);

	UFUNCTION()
	void MoveTowardsPlayer(float InForce);

	//Collider Functions
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Setters/Getters
	UFUNCTION()
	void SetXPValue(const int32 InXPValue) { XPValue = InXPValue; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "XP")
	int32 XPValue;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool bOuterBoost;

	class AMainCharacter* Player;
	float MinimumLifeTime;

	UFUNCTION()
	void RecheckInner();
	
};
