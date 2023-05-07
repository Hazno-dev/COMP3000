// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "Engine/PointLight.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "LootCrate.generated.h"

UCLASS()
class COMP3000_API ALootCrate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootCrate();

	//Tier value is between 0-2 (0 = lowest tier, 2 = highest tier)
	UFUNCTION()
	void EnableCrate(int Tier);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UPointLightComponent> Tier1Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UPointLightComponent> Tier2Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UNiagaraSystem> OpenVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* BaseCrateMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* Tier1CrateMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* Tier2CrateMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* OpenCrateMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TSubclassOf<class AXPOrb> XPOrbClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	UDataTable* LootDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	bool bForcedSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (EditCondition = "bForcedSpawn"))
	int ForcedSpawnTier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* OpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* OpenAttenuation;

	UFUNCTION()
	void OpenLootCrate();

	UPROPERTY()
	bool bIsOpen = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<struct FLootTable*> LootTable;
	int XPMin;
	int XPMax;

};
