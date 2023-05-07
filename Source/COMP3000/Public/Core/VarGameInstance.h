// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VarGameInstance.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelGenerationFinished);


UCLASS()
class COMP3000_API UVarGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	//Global delegate used to determine when the level is done generating
	UPROPERTY(BlueprintAssignable)
	FOnLevelGenerationFinished OnLevelGenerationFinished;
	
};
