// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AbilityBase.h"
#include "GameFramework/Actor.h"
#include "Heroes/Abilities/AbilityData.h"
#include "HeroGeneration.generated.h"


UENUM()
enum EHeroClasses
{
	Elementalist,
	Striker,
	Rogue,
	Paladin
};

USTRUCT(BlueprintType)
struct FHeroStats : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EHeroClasses> HeroClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxHP;

	UPROPERTY(BlueprintReadOnly)
	int32 XP;

	UPROPERTY(BlueprintReadOnly)
	int32 Level;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxDashCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DashDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class AAbilityBase>> ValidAbilities;

	FHeroStats()
	{
		HP = 3;
		MaxDashCount = 2;
		HeroClass = Elementalist;
		DashDuration = 0.15f;
		XP = 0;
	}

	
};

USTRUCT(BlueprintType)
struct FLevelXP : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 XPToNextLevel;

	FLevelXP()
	{
		Level = 0;
		XPToNextLevel = 100;
	}
	
};

UCLASS()
class COMP3000_API UHeroDataInstance : public UObject
{
	GENERATED_BODY()

public:
    /* Hero Generation Data */

    //Hero Stats
	UPROPERTY()
    FHeroStats HeroStats;
	
	//Ability Instances
	UPROPERTY()
	AAbilityBase* Ability1InstanceOG;

	UPROPERTY()
	AAbilityBase* Ability2InstanceOG;

	UPROPERTY()
	AAbilityBase* Ability3InstanceOG;

	UPROPERTY()
	AAbilityBase* UltimateAbilityInstanceOG;
	
	//Mesh Data
	//Gender Data
	//True = Male, False = Female
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	bool Gender;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GHeadInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GEyebrowsInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GFacialHairInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GTorsoInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GUpperArmLInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GUpperArmRInt;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GLowerArmLInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GLowerArmRInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GHandRInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GHandLInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GHipsInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GLegRInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int GLegLInt;

	//Generic Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AHeadCoverNoHairInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AHeadCoverBaseHairInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AHeadCoverNoFacialHairInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AHairInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AHeadAttachmentInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AChestAttachmentInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int ABackAttachmentInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AShoulderLAttachmentInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AShoulderRAttachmentInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AElbowLAttachmentInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AElbowRAttachmentInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AHipsAttachmentInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AKneeLAttachmentInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AKneeRAttachmentInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int AExtraInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int HeadCoveringInt;

	//Colour Ints
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int SkinColourInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int PrimaryColourInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int SecondaryColourInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int MetalPrimaryColourInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int MetalSecondaryColourInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int MetalDarkColourInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int LeatherPrimaryColourInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int LeatherSecondaryColourInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int HairColourInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generation Data")
	int BodyArtColourInt;
	
};

UCLASS()
class COMP3000_API AHeroGeneration : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeroGeneration();
	
	
};
