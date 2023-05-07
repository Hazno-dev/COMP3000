// Callum Stables, 2023


#include "UI/Player/PlayerHUD.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Core/VarGameInstance.h"
#include "Kismet/GameplayStatics.h"

void APlayerHUD::BeginPlay() {
	Super::BeginPlay();

	//if (bDebugWorld) return;

	StartLoadingScreen();

	UVarGameInstance* GameInstance = Cast<UVarGameInstance>(GetGameInstance());
	if (GameInstance) GameInstance->OnLevelGenerationFinished.AddDynamic(this, &APlayerHUD::LoadingScreenFinished);
	else {
		UE_LOG(LogTemp, Error, TEXT("GameInstance is null"));
		LoadingScreenFinished();
	}

	if (bDebugWorld) LoadingScreenFinished();
}

void APlayerHUD::StartLoadingScreen() {
	if (LoadingScreenClass) {
		LoadingScreenInstance = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenClass);
		LoadingScreenInstance->AddToViewport();
		
		PlayerController = GetWorld()->GetFirstPlayerController();

		if (PlayerController) {
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->bShowMouseCursor = true;
		}
	}
}

void APlayerHUD::LoadingScreenFinished() {

	if (LoadingScreenInstance) LoadingScreenInstance->RemoveFromParent();

	if (UMGHUDClass) {
		UMGHUDInstance = CreateWidget<UUserWidget>(GetWorld(), UMGHUDClass);
		UMGHUDInstance->AddToViewport();
	}

	if (LoadingScreenEndedClass) {
		LoadingScreenEndedInstance = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenEndedClass);
		LoadingScreenEndedInstance->AddToViewport();
		
		if (LoadingScreenSound && PlayerController) UGameplayStatics::PlaySound2D(PlayerController->GetWorld(), LoadingScreenSound, 0.4f);
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerHUD::LoadingScreenEndedFinished, 6.0f, false);
	}
	
	if (PlayerController) {
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, nullptr, EMouseLockMode::LockAlways, false);
		//PlayerController->SetInputMode(FInputModeGameOnly());
	}
	else {
		PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController) {
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, nullptr, EMouseLockMode::LockAlways, false);

		}
		else UE_LOG(LogTemp, Error, TEXT("PlayerController is null"));
	}
}

void APlayerHUD::LoadingScreenEndedFinished() {
	if (LoadingScreenEndedInstance) LoadingScreenEndedInstance->RemoveFromParent();
}
