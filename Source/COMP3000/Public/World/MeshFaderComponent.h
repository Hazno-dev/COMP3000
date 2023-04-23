// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeshFaderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class COMP3000_API UMeshFaderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMeshFaderComponent();

	//Tick
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<UMaterialInterface*> FadeMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	float FadeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Transparency;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float MinOpacity;
	
	UFUNCTION()
	void FadeIn();

	UFUNCTION()
	void FadeOut();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UFUNCTION()
	bool MaterialsValidityCheck(AStaticMeshActor* Actor, TArray<UMaterialInterface*> Materials);

	UFUNCTION()
	void SetFadeMaterials(AStaticMeshActor* Actor, TArray<UMaterialInstanceDynamic*> DMaterials);

	UFUNCTION()
	void ResetMaterials();
	
	UFUNCTION()
	void FadeInOutValueSet();

	UPROPERTY()
	float FadeInOutValue;

	UPROPERTY()
	AStaticMeshActor* SMActor;

	UPROPERTY()
	TArray<UMaterialInterface*> SMMaterials;
	
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> FadeMaterialDynamic;

	bool MeshFaded;
		
};
