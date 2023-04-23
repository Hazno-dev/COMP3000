// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldCursor.generated.h"

UCLASS()
class COMP3000_API AWorldCursor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldCursor();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Cursor")
	TObjectPtr<UDecalComponent> CursorDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TObjectPtr<UMaterialInterface> NormalMaterial;

	UFUNCTION()
	void MoveCursor(FVector Location, FVector Normal);

	UFUNCTION()
	void ScaleCursor(FVector Scale);

	UFUNCTION()
	void ToggleVisibility(bool bVisible);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:



};
