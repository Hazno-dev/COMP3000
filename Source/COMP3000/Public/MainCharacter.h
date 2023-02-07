// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileSpawner.h"
#include "GameFramework/Character.h"
#include "Heroes/HeroGeneration.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "MainCharacter.generated.h"


UENUM(BlueprintType)
enum class E_PlayerDirection : uint8 {
	NONE UMETA(DisplayName = "NONE"),
	LEFT UMETA(DisplayName = "LEFT"),
	RIGHT UMETA(DisplayName = "RIGHT")
};
UCLASS()
class COMP3000_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	//UNREAL DEFAULTS
	// Sets default values for this character's properties
	AMainCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind inputs
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	//COMPONENT DEFINITIONS
	/** Main Camera */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;
	
	/** Camera springarm */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

	/** Player Controller reference */
	UPROPERTY()
	APlayerController* SavedController;

	
	//PUBLIC PLAYER VARIABLES
	/** Character holding punch */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerController")
	bool Punching = false;

	/** Players rotation  */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerController")
	FRotator PublicRot = FRotator(0,0,0);

	/** Players Rotation Direction */
	UPROPERTY(EditAnywhere, Category = "PlayerController")
	TEnumAsByte<E_PlayerDirection> PlayerDirection;

	//PUBLIC PLAYER STATS
	/** Players Health */
	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	int PlayerHealth;
	
	//PLAYER MONTAGES
	/** Punching Montage  */
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* Mon_Punching;

	/** Punching Montage  */
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* Mon_Walking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionStimuliSourceComponent> AIStimuliSource;

	//Shooting
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UProjectileSpawner* ProjectileSpawner;

	UFUNCTION(BlueprintCallable)
	void ShootProjectile();

	//Heroes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroes")
	TArray<FHeroDataStruct> HeroDataArray;

	//Components
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
	
protected:

	/** Forward/Backwards motion */
	void MoveForward(float Value);

	/** Left/Right motion */
	void MoveRight(float Value);

	/** Start punching */
	void Punch();

	/** Stop punching */
	void StopPunch();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/** Trace Channel for Mouse Aiming */
	ETraceTypeQuery TraceChannel;
	
	/** Hitpoint for Mouse Aiming */
	FHitResult HitResult;
	
	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void GenerateHero(UPARAM(ref) FHeroDataStruct& InHero);

	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetMaterials();

	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetMaterialParameters(FHeroDataStruct InHero);

	UFUNCTION(BlueprintCallable, Category = "Generation Functions")
	void SetMeshes(FHeroDataStruct InHero);

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;
};
