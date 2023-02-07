// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "AssetDataTableStruct.generated.h"
/**
 * 
 */


USTRUCT(BlueprintType)
struct FAssetDataTable : public FTableRowBase
{

	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* Hat;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* Hair;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* Head;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* FacialHair;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* Torso;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* Hip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* Leg;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* Arms;
};
