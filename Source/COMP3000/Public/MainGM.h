// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/Player/PlayerHUD.h"
#include "MainGM.generated.h"

/**
 * 
 */
UCLASS()
class COMP3000_API AMainGM : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGM();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APlayerHUD> PlayerHUDClass;

/*protected:
	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	UUserWidget* UMGHUDInstance;

	virtual void BeginPlay() override;*/
};
