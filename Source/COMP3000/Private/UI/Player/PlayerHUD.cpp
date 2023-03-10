// Callum Stables, 2023


#include "UI/Player/PlayerHUD.h"

#include "Blueprint/UserWidget.h"

void APlayerHUD::BeginPlay() {
	Super::BeginPlay();

	if (UMGHUDClass) {
		UMGHUDInstance = CreateWidget<UUserWidget>(GetWorld(), UMGHUDClass);
		UMGHUDInstance->AddToViewport();
	}
}
