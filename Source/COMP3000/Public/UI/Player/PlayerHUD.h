// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Animation/WidgetAnimation.h"
#include "GameFramework/HUD.h"
#include "Sound/SoundCue.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<UUserWidget> LoadingScreenClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<UUserWidget> LoadingScreenEndedClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Audio)
	USoundCue* LoadingScreenSound;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	UUserWidget* UMGHUDInstance;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	UUserWidget* LoadingScreenInstance;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	UUserWidget* LoadingScreenEndedInstance;

	UFUNCTION()
	void StartLoadingScreen();
	
	UFUNCTION()
	void LoadingScreenFinished();

	UFUNCTION()
	void LoadingScreenEndedFinished();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Properties)
	bool bDebugWorld;
	
protected:
	virtual void BeginPlay() override;

private:
	APlayerController* PlayerController;
	
};
