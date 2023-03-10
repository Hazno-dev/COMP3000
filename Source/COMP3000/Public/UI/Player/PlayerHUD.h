// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<UUserWidget> UMGHUDClass;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	UUserWidget* UMGHUDInstance;

protected:
	virtual void BeginPlay() override;
};
