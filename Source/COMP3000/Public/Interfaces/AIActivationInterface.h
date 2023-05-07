// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIActivationInterface.generated.h"

/**
 * 
 */
UINTERFACE()
class COMP3000_API UAIActivationInterface : public UInterface
{
	GENERATED_BODY()
};

class COMP3000_API IAIActivationInterface
{
	GENERATED_BODY()

public:
	virtual void ActivateAI() = 0;
	virtual void DeactivateAI() = 0;

	virtual void InnerAIActivation() = 0;
	virtual void InnerAIDeactivation() = 0;
};