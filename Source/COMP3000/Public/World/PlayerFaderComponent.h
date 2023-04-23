// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "MeshFaderComponent.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerFaderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMP3000_API UPlayerFaderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerFaderComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float TickRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float TraceSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float TraceHeightOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float TraceDistanceFromPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	TEnumAsByte<ETraceTypeQuery> TraceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool DebugFadeTrace;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class AMainCharacter* Player;

	UPROPERTY()
	class UCameraComponent* PlayerCamera;
	
	EDrawDebugTrace::Type DebugTraceType;

	UPROPERTY()
	TArray<UMeshFaderComponent*> CurrentFaders;
	
};
