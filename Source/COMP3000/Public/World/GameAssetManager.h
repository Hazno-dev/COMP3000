// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GameAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API UGameAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	virtual void StartInitialLoading() override;
	
};
