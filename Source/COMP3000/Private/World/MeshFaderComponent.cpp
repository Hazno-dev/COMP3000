// Callum Stables, 2023


#include "World/MeshFaderComponent.h"

#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UMeshFaderComponent::UMeshFaderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	RegisterComponent();
	PrimaryComponentTick.Target = this;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
	
	Transparency = 0.95;
	FadeSpeed = 3.0;
	MinOpacity = 0.0;
}


// Called when the game starts
void UMeshFaderComponent::BeginPlay()
{
	Super::BeginPlay();

	PrimaryComponentTick.RegisterTickFunction(GetComponentLevel());

	SMActor = Cast<AStaticMeshActor>(GetOwner());
	if (IsValid(SMActor)) SMMaterials = SMActor->GetStaticMeshComponent()->GetMaterials();
	
	SetComponentTickEnabled(false);
}

// Called every frame
void UMeshFaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FadeInOutValueSet();
	
	float OpacityValue = UKismetMathLibrary::FInterpEaseInOut(MinOpacity, Transparency, FadeInOutValue, 2.0);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::SanitizeFloat(OpacityValue));

	for (UMaterialInstanceDynamic* Material : FadeMaterialDynamic) {
		if (!IsValid(Material)) continue;
		Material->SetScalarParameterValue("Opacity", OpacityValue);
	}

	if (MeshFaded && FadeInOutValue >= 1.0) SetComponentTickEnabled(false);
	else if (!MeshFaded && FadeInOutValue <= 0.0) {
		SetComponentTickEnabled(false);
		ResetMaterials();
		
	}
}

void UMeshFaderComponent::FadeIn() {
	if (!MaterialsValidityCheck(SMActor, FadeMaterials)) return;

	if (FadeMaterialDynamic.Num() == 0) {
		for (UMaterialInterface* Material : FadeMaterials) {
			FadeMaterialDynamic.Add(UMaterialInstanceDynamic::Create(Material, this));
		}
	}

	if (FadeMaterialDynamic.Num() > 0) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Fade In Num over 0"));
		MeshFaded = true;
		SetComponentTickEnabled(true);
		SetFadeMaterials(SMActor, FadeMaterialDynamic);
	}
}

void UMeshFaderComponent::FadeOut() {
	if (!MaterialsValidityCheck(SMActor, SMMaterials)) return;
	
	if (FadeMaterialDynamic.Num() > 0) {
		MeshFaded = false;
		SetComponentTickEnabled(true);
	}
}

bool UMeshFaderComponent::MaterialsValidityCheck(AStaticMeshActor* Actor, TArray<UMaterialInterface*> Materials) {
	if (!IsValid(Actor)) return false;
	if (Materials.Num() == 0) return false;
	return true;
}

void UMeshFaderComponent::SetFadeMaterials(AStaticMeshActor* Actor, TArray<UMaterialInstanceDynamic*> DMaterials) {
	if (!IsValid(Actor)) return;
	if (DMaterials.Num() == 0) return;
	if (!MaterialsValidityCheck(Actor, SMMaterials)) return;
	if (!IsValid(Actor->GetStaticMeshComponent())) return;
	
	for (int i = 0; i < DMaterials.Num(); i++) {
		if (i >= SMMaterials.Num()) break;
		
		UMaterialInstanceDynamic* DynamicMaterial = DMaterials[i];
		if (!IsValid(DynamicMaterial)) continue;
		
		Actor->GetStaticMeshComponent()->SetMaterial(i, DynamicMaterial);
	}

	if (DMaterials.Num() < SMMaterials.Num()) {
		UMaterialInstanceDynamic* DynamicMaterial = DMaterials[0];
		if (!IsValid(DynamicMaterial)) return;
		
		for (int i = 0; i < SMMaterials.Num(); i++) {
			if (i >= SMMaterials.Num()) break;
			Actor->GetStaticMeshComponent()->SetMaterial(i, DynamicMaterial);
		}
	}
		
}

void UMeshFaderComponent::ResetMaterials() {
	if (!MaterialsValidityCheck(SMActor, SMMaterials)) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Reset Materials"));
	
	for (int i = 0; i < SMMaterials.Num(); i++) SMActor->GetStaticMeshComponent()->SetMaterial(i, SMMaterials[i]);
	
}

void UMeshFaderComponent::FadeInOutValueSet() {
	if (!IsValid(GetWorld())) return;
	
	if (MeshFaded) FadeInOutValue = UKismetMathLibrary::FInterpTo_Constant(FadeInOutValue, 1.0, GetWorld()->GetDeltaSeconds(), FadeSpeed);
	else FadeInOutValue = UKismetMathLibrary::FInterpTo_Constant(FadeInOutValue, 0.0, GetWorld()->GetDeltaSeconds(), FadeSpeed);
}


