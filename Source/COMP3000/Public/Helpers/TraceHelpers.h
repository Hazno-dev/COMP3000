// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TraceHelpers.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UTraceHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//Raycasters
	UFUNCTION(BlueprintCallable, Category = "Trace")
	static FVector TraceGroundLocation(AActor* Actor, float ZOffset, float TraceDistance);
	
};
