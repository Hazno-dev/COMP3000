// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGM.h"

#include "MainCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UI/Player/PlayerHUD.h"


AMainGM::AMainGM() {
	PrimaryActorTick.bCanEverTick = false;

	DefaultPawnClass = nullptr;

	HUDClass = PlayerHUDClass;
	
}

/*void AMainGM::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(UMGHUDClass)) {
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController == nullptr) return;
		UMGHUDInstance = CreateWidget<UUserWidget>(GetWorld(), UMGHUDClass);
		UMGHUDInstance->AddToViewport();
	}

}*/
