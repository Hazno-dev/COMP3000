// Callum Stables, 2023
// Unlikely to be used, but just in case //DEPRECATED 

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Components/ActorComponent.h"
#include "HeroGenerator.generated.h"


class AMainCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMP3000_API UHeroGenerator : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHeroGenerator();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero Stats")
	UDataTable* HeroStatsDataTable;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroes")
	TArray<FHeroDataStruct> HeroDataArray;*/

	//Generation Functions	
	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	TArray<UHeroDataInstance*> GenerateHero();

	//Mesh Public Functions
	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetMeshesHidden(bool bIsHidden);

	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetMeshes(UHeroDataInstance* InHero);

	UFUNCTION()
	void SetupIcons(TArray<UHeroDataInstance*>& InHeroDataArray);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//Mesh Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AHeadCoveringNoHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AHeadAttachmentHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AHeadAttachmentHelmet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AChestAttachment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AHeadCoveringBaseHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AHeadCoveringNoFacialHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> ABackAttachment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AShoulderAttachmentL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AShoulderAttachmentR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AElbowAttachmentL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AElbowAttachmentR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AHipAttachment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AKneeAttachmentL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AKneeAttachmentR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> AExtra;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GLegL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GLegR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GHead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GEyebrows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GFacialHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GTorso;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GArmUpperR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GArmUpperL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GArmLowerR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GArmLowerL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GHandR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GHandL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> GHips;
	
	/* Asset References*/

	//Generic Assets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AHeadCoverNoHairArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AHeadCoverBaseHairArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AHeadCoverNoFacialHairArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AHairArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AHeadAttachmentHairArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AHeadAttachmentHelmetArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AChestAttachmentArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> ABackAttachmentArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AShoulderLAttachmentArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AShoulderRAttachmentArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AElbowLAttachmentArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AElbowRAttachmentArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AHipsAttachmentArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AKneeLAttachmentArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AKneeRAttachmentArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> AExtraArray;
	
	//Male Assets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MHeadArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MEyebrowsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MFacialHairArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MTorsoArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MUpperArmLArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MUpperArmRArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MLowerArmLArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MLowerArmRArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MHandRArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MHandLArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MHipsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MLegRArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> MLegLArray;

	//Female Assets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FHeadArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FEyebrowsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FFacialHairArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FTorsoArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FUpperArmLArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FUpperArmRArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FLowerArmLArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FLowerArmRArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FHandRArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FHandLArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FHipsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FLegRArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<USkeletalMesh*> FLegLArray;

	//Colour Arrays
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<FLinearColor> SkinColourArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<FLinearColor> PrimaryColourArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<FLinearColor> SecondaryColourArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<FLinearColor> MetalPrimaryColourArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<FLinearColor> MetalSecondaryColourArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<FLinearColor> MetalDarkColourArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<FLinearColor> LeatherPrimaryColourArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<FLinearColor> LeatherSecondaryColourArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<FLinearColor> BodyArtColourArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<FLinearColor> HairColourArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<FLinearColor> StubbleColourArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	TArray<FLinearColor> ScarsColourArray;

	//Material Reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	UMaterialInterface* MaterialReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Data")
	UMaterialInterface* MaskReference;

private:

	//Main Character Reference
	AMainCharacter* MainCharacterRef;
	USkeletalMeshComponent* GetMesh() const {return MainCharacterRef->GetMesh();};

	TArray<FHeroStats*> DataTableStatsRef;

	//Mesh Material
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetupMeshes();
	
	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetupMeshAttachments();

	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void GenerateHeroMeshes(UPARAM(ref) UHeroDataInstance* InHero);

	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetMaterials();

	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetMaterialParameters(UPARAM(ref)UHeroDataInstance* InHero);

	void SetupAttachmentRuntime();
};



