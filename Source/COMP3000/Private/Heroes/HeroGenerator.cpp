// Callum Stables, 2023
// Unlikely to be used, but just in case //DEPRECATED 


#include "Heroes/HeroGenerator.h"

#include "MainCharacter.h"
#include "SkeletalMeshMerge.h"
#include "Helpers/ArrayHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UHeroGenerator::UHeroGenerator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	SetupMeshes();
	MainCharacterRef = Cast<AMainCharacter>(GetOwner());
	if (MainCharacterRef) SetupAttachmentRuntime();
	
}

// Called when the game starts
void UHeroGenerator::BeginPlay()
{
	Super::BeginPlay();

	/*
	MainCharacterRef = Cast<AMainCharacter>(GetOwner());
	if (!MainCharacterRef) return;

	GetMesh()->SetVisibility(false, false);
	
	SetupAttachmentRuntime();
	DynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(0, MaterialReference);
	SetMaterials();

	HeroStatsDataTable->GetAllRows<FHeroStats>("FHeroStats", DataTableStatsRef);
	*/

	/*for (int i = 0; i < HeroDataArray.Num(); i++) {
		GenerateHero(HeroDataArray[i]);
	}
	
	SetMeshes(HeroDataArray[0]);*/
	
}


// Called every frame
void UHeroGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<UHeroDataInstance*> UHeroGenerator::GenerateHero() {

	MainCharacterRef = Cast<AMainCharacter>(GetOwner());
	if (!MainCharacterRef) return TArray<UHeroDataInstance*>(); //Return empty array if no main character is set

	GetMesh()->SetVisibility(false, false);
	
	SetupAttachmentRuntime();
	DynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(0, MaterialReference);
	SetMaterials();
	

	if (!IsValid(HeroStatsDataTable)) return TArray<UHeroDataInstance*>(); //Return empty array if no data table is set

	TArray<FHeroStats*> DataTableStats;
	HeroStatsDataTable->GetAllRows<FHeroStats>(TEXT("GetAllRows in GenerateHero"), DataTableStats);

	
	TArray<UHeroDataInstance*> ReturnHDI;
	
	//Hero Stats
	for (int i  = 0; i < 4; i++) {
		UHeroDataInstance* HDI = NewObject<UHeroDataInstance>(this);
		
		if (DataTableStats[i])
		{
			HDI->HeroStats = *DataTableStats[i];
		} 
		
		GenerateHeroMeshes(HDI);
		ReturnHDI.Add(HDI);
	}

	SetMeshes(ReturnHDI[0]);

	return ReturnHDI;
}

void UHeroGenerator::SetupMeshes() {
	AHeadCoveringNoHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHeadCoveringNoHair"));
	AHeadCoveringNoHair->SetMobility(EComponentMobility::Movable);
	AHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHair"));
	AHair->SetMobility(EComponentMobility::Movable);
	AHeadAttachmentHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHeadAttachmentHair"));
	AHeadAttachmentHair->SetMobility(EComponentMobility::Movable);
	AHeadAttachmentHelmet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHeadAttachmentHelmet"));
	AHeadAttachmentHelmet->SetMobility(EComponentMobility::Movable);
	AChestAttachment = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AChestAttachment"));
	AChestAttachment->SetMobility(EComponentMobility::Movable);
	AHeadCoveringBaseHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHeadCoveringBaseHair"));
	AHeadCoveringBaseHair->SetMobility(EComponentMobility::Movable);
	AHeadCoveringNoFacialHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHeadCoveringNoFacialHair"));
	AHeadCoveringNoFacialHair->SetMobility(EComponentMobility::Movable);
	ABackAttachment = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ABackAttachment"));
	ABackAttachment->SetMobility(EComponentMobility::Movable);
	AShoulderAttachmentL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AShoulderAttachmentL"));
	AShoulderAttachmentL->SetMobility(EComponentMobility::Movable);
	AShoulderAttachmentR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AShoulderAttachmentR"));
	AShoulderAttachmentR->SetMobility(EComponentMobility::Movable);
	AElbowAttachmentL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AElbowAttachmentL"));
	AElbowAttachmentL->SetMobility(EComponentMobility::Movable);
	AElbowAttachmentR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AElbowAttachmentR"));
	AElbowAttachmentR->SetMobility(EComponentMobility::Movable);
	AHipAttachment = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AHipAttachment"));
	AHipAttachment->SetMobility(EComponentMobility::Movable);
	AKneeAttachmentL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AKneeAttachmentL"));
	AKneeAttachmentL->SetMobility(EComponentMobility::Movable);
	AKneeAttachmentR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AKneeAttachmentR"));
	AKneeAttachmentR->SetMobility(EComponentMobility::Movable);
	AExtra	= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AExtra"));
	AExtra->SetMobility(EComponentMobility::Movable);
	GLegL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GLegL"));
	GLegL->SetMobility(EComponentMobility::Movable);
	GLegR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GLegR"));
	GLegR->SetMobility(EComponentMobility::Movable);
	GHead = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GHead"));
	GHead->SetMobility(EComponentMobility::Movable);
	GEyebrows = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GEyebrows"));
	GEyebrows->SetMobility(EComponentMobility::Movable);
	GFacialHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GFacialHair"));
	GFacialHair->SetMobility(EComponentMobility::Movable);
	GTorso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GTorso"));
	GTorso->SetMobility(EComponentMobility::Movable);
	GArmUpperR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GArmUpperR"));
	GArmUpperR->SetMobility(EComponentMobility::Movable);
	GArmUpperL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GArmUpperL"));
	GArmUpperL->SetMobility(EComponentMobility::Movable);
	GArmLowerR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GArmLowerR"));
	GArmLowerR->SetMobility(EComponentMobility::Movable);
	GArmLowerL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GArmLowerL"));
	GArmLowerL->SetMobility(EComponentMobility::Movable);
	GHandR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GHandR"));
	GHandR->SetMobility(EComponentMobility::Movable);
	GHandL = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GHandL"));
	GHandL->SetMobility(EComponentMobility::Movable);
	GHips = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GHips"));
	GHips->SetMobility(EComponentMobility::Movable);
	
}

void UHeroGenerator::SetupMeshAttachments() {
	AHeadCoveringNoHair->SetupAttachment(GetMesh());
	AHair->SetupAttachment(GetMesh());
	AHeadAttachmentHair->SetupAttachment(GetMesh());
	AHeadAttachmentHelmet->SetupAttachment(GetMesh());
	AChestAttachment->SetupAttachment(GetMesh());
	AHeadCoveringBaseHair->SetupAttachment(GetMesh());
	AHeadCoveringNoFacialHair->SetupAttachment(GetMesh());
	ABackAttachment->SetupAttachment(GetMesh(), "spine_03");
	AShoulderAttachmentL->SetupAttachment(GetMesh());
	AShoulderAttachmentR->SetupAttachment(GetMesh());
	AElbowAttachmentL->SetupAttachment(GetMesh());
	AElbowAttachmentR->SetupAttachment(GetMesh());
	AHipAttachment->SetupAttachment(GetMesh());
	AKneeAttachmentL->SetupAttachment(GetMesh());
	AKneeAttachmentR->SetupAttachment(GetMesh());
	AExtra->SetupAttachment(GetMesh());

	GLegL->SetupAttachment(GetMesh());
	GLegR->SetupAttachment(GetMesh());
	GHead->SetupAttachment(GetMesh());
	GEyebrows->SetupAttachment(GetMesh());
	GFacialHair->SetupAttachment(GetMesh());
	GTorso->SetupAttachment(GetMesh());
	GArmUpperR->SetupAttachment(GetMesh());
	GArmUpperL->SetupAttachment(GetMesh());

	GArmLowerR->SetupAttachment(GetMesh());
	GArmLowerL->SetupAttachment(GetMesh());
	GHandR->SetupAttachment(GetMesh());
	GHandL->SetupAttachment(GetMesh());
	GHips->SetupAttachment(GetMesh());
}

void UHeroGenerator::SetupAttachmentRuntime() {
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AHeadCoveringNoHair->AttachToComponent(GetMesh(), AttachmentRules);
	AHair->AttachToComponent(GetMesh(), AttachmentRules);
	AHeadAttachmentHair->AttachToComponent(GetMesh(), AttachmentRules);
	AHeadAttachmentHelmet->AttachToComponent(GetMesh(), AttachmentRules);
	AChestAttachment->AttachToComponent(GetMesh(), AttachmentRules);
	AHeadCoveringBaseHair->AttachToComponent(GetMesh(), AttachmentRules);
	AHeadCoveringNoFacialHair->AttachToComponent(GetMesh(), AttachmentRules);
	ABackAttachment->AttachToComponent(GetMesh(), AttachmentRules, "spine_03");
	ABackAttachment->SetRelativeLocation(FVector(-130,25,0));
	ABackAttachment->SetRelativeRotation(FRotator(-90, 0, -10));
	AShoulderAttachmentL->AttachToComponent(GetMesh(), AttachmentRules);
	AShoulderAttachmentR->AttachToComponent(GetMesh(), AttachmentRules);
	AElbowAttachmentL->AttachToComponent(GetMesh(), AttachmentRules);
	AElbowAttachmentR->AttachToComponent(GetMesh(), AttachmentRules);
	AHipAttachment->AttachToComponent(GetMesh(), AttachmentRules);
	AKneeAttachmentL->AttachToComponent(GetMesh(), AttachmentRules);
	AKneeAttachmentR->AttachToComponent(GetMesh(), AttachmentRules);
	AExtra->AttachToComponent(GetMesh(), AttachmentRules);

	GLegL->AttachToComponent(GetMesh(), AttachmentRules);
	GLegR->AttachToComponent(GetMesh(), AttachmentRules);
	GHead->AttachToComponent(GetMesh(), AttachmentRules);
	GEyebrows->AttachToComponent(GetMesh(), AttachmentRules);
	GFacialHair->AttachToComponent(GetMesh(), AttachmentRules);
	GTorso->AttachToComponent(GetMesh(), AttachmentRules);
	GArmUpperR->AttachToComponent(GetMesh(), AttachmentRules);
	GArmUpperL->AttachToComponent(GetMesh(), AttachmentRules);

	GArmLowerR->AttachToComponent(GetMesh(), AttachmentRules);
	GArmLowerL->AttachToComponent(GetMesh(), AttachmentRules);
	GHandR->AttachToComponent(GetMesh(), AttachmentRules);
	GHandL->AttachToComponent(GetMesh(), AttachmentRules);
	GHips->AttachToComponent(GetMesh(), AttachmentRules);
	
}

void UHeroGenerator::GenerateHeroMeshes(UHeroDataInstance* InHero) {
	//Mesh Data
	//Gendered Data
	InHero->Gender = UKismetMathLibrary::RandomBool();
	
	InHero->GHeadInt = UKismetMathLibrary::RandomIntegerInRange(0, MHeadArray.Num() - 1);

	if (InHero->Gender) {
		InHero->GEyebrowsInt = UKismetMathLibrary::RandomIntegerInRange(0, MEyebrowsArray.Num() - 1);
		InHero->GFacialHairInt = UKismetMathLibrary::RandomIntegerInRange(0, MFacialHairArray.Num() - 1);
	}
	else {
		InHero->GEyebrowsInt = UKismetMathLibrary::RandomIntegerInRange(0, FEyebrowsArray.Num() - 1);
		InHero->GFacialHairInt = 0;
	}

	InHero->GTorsoInt = UKismetMathLibrary::RandomIntegerInRange(0, MTorsoArray.Num() - 1);
	
	InHero->GUpperArmLInt = UKismetMathLibrary::RandomIntegerInRange(0, MUpperArmLArray.Num() - 1);
	UKismetMathLibrary::RandomBoolWithWeight(0.8) ? InHero->GUpperArmRInt = InHero->GUpperArmLInt :
		InHero->GUpperArmRInt = UKismetMathLibrary::RandomIntegerInRange(0, MUpperArmRArray.Num() - 1);

	InHero->GLowerArmLInt = UKismetMathLibrary::RandomIntegerInRange(0, MLowerArmLArray.Num() - 1);
	UKismetMathLibrary::RandomBoolWithWeight(0.8) ? InHero->GLowerArmRInt = InHero->GLowerArmLInt :
		InHero->GLowerArmRInt = UKismetMathLibrary::RandomIntegerInRange(0, MLowerArmRArray.Num() - 1);

	InHero->GHandLInt = UKismetMathLibrary::RandomIntegerInRange(0, MHandLArray.Num() - 1);
	UKismetMathLibrary::RandomBoolWithWeight(0.8) ? InHero->GHandRInt = InHero->GHandLInt :
		InHero->GHandRInt = UKismetMathLibrary::RandomIntegerInRange(0, MHandRArray.Num() - 1);

	InHero->GHipsInt = UKismetMathLibrary::RandomIntegerInRange(0, MHipsArray.Num() - 1);

	InHero->GLegLInt = UKismetMathLibrary::RandomIntegerInRange(0, MLegLArray.Num() - 1);
	UKismetMathLibrary::RandomBoolWithWeight(0.8) ? InHero->GLegRInt = InHero->GLegLInt :
		InHero->GLegRInt = UKismetMathLibrary::RandomIntegerInRange(0, MLegRArray.Num() - 1);

	// Generic Data
	InHero->HeadCoveringInt = UKismetMathLibrary::RandomIntegerInRange(0, 3);
	
	InHero->AHeadCoverBaseHairInt = UKismetMathLibrary::RandomIntegerInRange(0, AHeadCoverBaseHairArray.Num() - 1);

	InHero->AHeadCoverNoFacialHairInt = UKismetMathLibrary::RandomIntegerInRange(0, AHeadCoverNoFacialHairArray.Num() - 1);

	InHero->AHeadCoverNoHairInt = UKismetMathLibrary::RandomIntegerInRange(0, AHeadCoverNoHairArray.Num() - 1);

	InHero->AHairInt = UKismetMathLibrary::RandomIntegerInRange(0, AHairArray.Num() - 1);

	InHero->AHeadAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AHeadAttachmentHelmetArray.Num() - 1);

	InHero->ABackAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, ABackAttachmentArray.Num() - 1);

	InHero->AShoulderLAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AShoulderLAttachmentArray.Num() - 1);

	InHero->AShoulderRAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AShoulderRAttachmentArray.Num() - 1);

	InHero->AElbowLAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AElbowLAttachmentArray.Num() - 1);

	InHero->AElbowRAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AElbowRAttachmentArray.Num() - 1);

	InHero->AHipsAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AHipsAttachmentArray.Num() - 1);

	InHero->AKneeLAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AKneeLAttachmentArray.Num() - 1);

	InHero->AKneeRAttachmentInt = UKismetMathLibrary::RandomIntegerInRange(0, AKneeRAttachmentArray.Num() - 1);

	UKismetMathLibrary::RandomBoolWithWeight(0.8) ? InHero->AExtraInt = 0 :
		InHero->AExtraInt = UKismetMathLibrary::RandomIntegerInRange(0, AExtraArray.Num() - 1);

	//Colour Data
	InHero->SkinColourInt = UKismetMathLibrary::RandomIntegerInRange(0, SkinColourArray.Num() - 1);

	InHero->PrimaryColourInt = UKismetMathLibrary::RandomIntegerInRange(0, PrimaryColourArray.Num() - 1);

	InHero->SecondaryColourInt = UKismetMathLibrary::RandomIntegerInRange(0, SecondaryColourArray.Num() - 1);

	InHero->MetalPrimaryColourInt = UKismetMathLibrary::RandomIntegerInRange(0, MetalPrimaryColourArray.Num() - 1);

	InHero->MetalSecondaryColourInt = UKismetMathLibrary::RandomIntegerInRange(0, MetalSecondaryColourArray.Num() - 1);

	InHero->MetalDarkColourInt = UKismetMathLibrary::RandomIntegerInRange(0, MetalDarkColourArray.Num() - 1);

	InHero->LeatherPrimaryColourInt = UKismetMathLibrary::RandomIntegerInRange(0, LeatherPrimaryColourArray.Num() - 1);

	InHero->LeatherSecondaryColourInt = UKismetMathLibrary::RandomIntegerInRange(0, LeatherSecondaryColourArray.Num() - 1);

	InHero->HairColourInt = UKismetMathLibrary::RandomIntegerInRange(0, HairColourArray.Num() - 1);

	InHero->BodyArtColourInt = UKismetMathLibrary::RandomIntegerInRange(0, BodyArtColourArray.Num() - 1);
}

void UHeroGenerator::SetMaterials() {
	GHead->SetMaterial(0, DynamicMaterial);
	GEyebrows->SetMaterial(0, DynamicMaterial);
	GFacialHair->SetMaterial(0, DynamicMaterial);
	GTorso->SetMaterial(0, DynamicMaterial);
	GArmUpperL->SetMaterial(0, DynamicMaterial);
	GArmUpperR->SetMaterial(0, DynamicMaterial);
	GArmLowerL->SetMaterial(0, DynamicMaterial);
	GArmLowerR->SetMaterial(0, DynamicMaterial);
	GHandL->SetMaterial(0, DynamicMaterial);
	GHandR->SetMaterial(0, DynamicMaterial);
	GHips->SetMaterial(0, DynamicMaterial);
	GLegL->SetMaterial(0, DynamicMaterial);
	GLegR->SetMaterial(0, DynamicMaterial);
	AHeadCoveringNoHair->SetMaterial(0, DynamicMaterial);
	AHair->SetMaterial(0, DynamicMaterial);
	AHeadAttachmentHair->SetMaterial(0, DynamicMaterial);
	AHeadAttachmentHelmet->SetMaterial(0, DynamicMaterial);
	AChestAttachment->SetMaterial(0, DynamicMaterial);
	AHeadCoveringBaseHair->SetMaterial(0, DynamicMaterial);
	AHeadCoveringNoFacialHair->SetMaterial(0, DynamicMaterial);
	ABackAttachment->SetMaterial(0, DynamicMaterial);
	AShoulderAttachmentL->SetMaterial(0, DynamicMaterial);
	AShoulderAttachmentR->SetMaterial(0, DynamicMaterial);
	AElbowAttachmentL->SetMaterial(0, DynamicMaterial);
	AElbowAttachmentR->SetMaterial(0, DynamicMaterial);
	AHipAttachment->SetMaterial(0, DynamicMaterial);
	AKneeAttachmentL->SetMaterial(0, DynamicMaterial);
	AKneeAttachmentR->SetMaterial(0, DynamicMaterial);
	AExtra->SetMaterial(0, DynamicMaterial);
}

void UHeroGenerator::SetMaterialParameters(UPARAM(ref) UHeroDataInstance*InHero) {
	
	DynamicMaterial->SetVectorParameterValue("Color_Skin",
		SkinColourArray[UKismetMathLibrary::ClampInt64(InHero->SkinColourInt, 0, SkinColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Stubble",
		StubbleColourArray[UKismetMathLibrary::ClampInt64(InHero->SkinColourInt, 0, StubbleColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Scar",
		ScarsColourArray[UKismetMathLibrary::ClampInt64(InHero->SkinColourInt, 0, ScarsColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Primary",
		PrimaryColourArray[UKismetMathLibrary::ClampInt64(InHero->PrimaryColourInt, 0, PrimaryColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Secondary",
		SecondaryColourArray[UKismetMathLibrary::ClampInt64(InHero->SecondaryColourInt, 0, SecondaryColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Metal_Primary",
		MetalPrimaryColourArray[UKismetMathLibrary::ClampInt64(InHero->MetalPrimaryColourInt, 0, MetalPrimaryColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Metal_Secondary",
		MetalSecondaryColourArray[UKismetMathLibrary::ClampInt64(InHero->MetalSecondaryColourInt, 0, MetalSecondaryColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Metal_Dark",
		MetalDarkColourArray[UKismetMathLibrary::ClampInt64(InHero->MetalDarkColourInt, 0, MetalDarkColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Leather_Primary",
		LeatherPrimaryColourArray[UKismetMathLibrary::ClampInt64(InHero->LeatherPrimaryColourInt, 0, LeatherPrimaryColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Leather_Secondary",
		LeatherSecondaryColourArray[UKismetMathLibrary::ClampInt64(InHero->LeatherSecondaryColourInt, 0, LeatherSecondaryColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_Hair",
		HairColourArray[UKismetMathLibrary::ClampInt64(InHero->HairColourInt, 0, HairColourArray.Num() - 1)]);

	DynamicMaterial->SetVectorParameterValue("Color_BodyArt",
		BodyArtColourArray[UKismetMathLibrary::ClampInt64(InHero->BodyArtColourInt, 0, BodyArtColourArray.Num() - 1)]);
	
}

void UHeroGenerator::SetMeshes(UHeroDataInstance* InHero) {
	//GetMesh()->SetMaterial(0, MaskReference);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SetMeshes")));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("InHero->GHeadInt %d"), InHero->GHeadInt));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MHeadArray.Num() %d"), MHeadArray.Num()));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("UKismetMathLibrary::Clamp(InHero->GHeadInt, 0, MHeadArray.Num() - 1) %d"), UKismetMathLibrary::Clamp(InHero->GHeadInt, 0, MHeadArray.Num() - 1)));

	SetMaterialParameters(InHero);

	if (InHero->Gender) {
		GHead->SetSkeletalMesh(MHeadArray[UKismetMathLibrary::Clamp(InHero->GHeadInt, 0, MHeadArray.Num() - 1)]);
		GEyebrows->SetSkeletalMesh(MEyebrowsArray[UKismetMathLibrary::Clamp(InHero->GEyebrowsInt, 0, MEyebrowsArray.Num() - 1)]);
		GFacialHair->SetSkeletalMesh(MFacialHairArray[UKismetMathLibrary::Clamp(InHero->GFacialHairInt, 0, MFacialHairArray.Num() - 1)]);
		GTorso->SetSkeletalMesh(MTorsoArray[UKismetMathLibrary::Clamp(InHero->GTorsoInt, 0, MTorsoArray.Num() - 1)]);
		GArmUpperL->SetSkeletalMesh(MUpperArmLArray[UKismetMathLibrary::Clamp(InHero->GUpperArmLInt, 0, MUpperArmLArray.Num() - 1)]);
		GArmUpperR->SetSkeletalMesh(MUpperArmRArray[UKismetMathLibrary::Clamp(InHero->GUpperArmRInt, 0, MUpperArmRArray.Num() - 1)]);
		GArmLowerL->SetSkeletalMesh(MLowerArmLArray[UKismetMathLibrary::Clamp(InHero->GLowerArmLInt, 0, MLowerArmLArray.Num() - 1)]);
		GArmLowerR->SetSkeletalMesh(MLowerArmRArray[UKismetMathLibrary::Clamp(InHero->GLowerArmRInt, 0, MLowerArmRArray.Num() - 1)]);
		GHandL->SetSkeletalMesh(MHandLArray[UKismetMathLibrary::Clamp(InHero->GHandLInt, 0, MHandLArray.Num() - 1)]);
		GHandR->SetSkeletalMesh(MHandRArray[UKismetMathLibrary::Clamp(InHero->GHandRInt, 0, MHandRArray.Num() - 1)]);
		GHips->SetSkeletalMesh(MHipsArray[UKismetMathLibrary::Clamp(InHero->GHipsInt, 0, MHipsArray.Num() - 1)]);
		GLegL->SetSkeletalMesh(MLegLArray[UKismetMathLibrary::Clamp(InHero->GLegLInt, 0, MLegLArray.Num() - 1)]);
		GLegR->SetSkeletalMesh(MLegRArray[UKismetMathLibrary::Clamp(InHero->GLegRInt, 0, MLegRArray.Num() - 1)]);
	} else {
		GHead->SetSkeletalMesh(FHeadArray[UKismetMathLibrary::Clamp(InHero->GHeadInt, 0, FHeadArray.Num() - 1)]);
		GEyebrows->SetSkeletalMesh(FEyebrowsArray[UKismetMathLibrary::Clamp(InHero->GEyebrowsInt, 0, FEyebrowsArray.Num() - 1)]);
		GTorso->SetSkeletalMesh(FTorsoArray[UKismetMathLibrary::Clamp(InHero->GTorsoInt, 0, FTorsoArray.Num() - 1)]);
		GArmUpperL->SetSkeletalMesh(FUpperArmLArray[UKismetMathLibrary::Clamp(InHero->GUpperArmLInt, 0, FUpperArmLArray.Num() - 1)]);
		GArmUpperR->SetSkeletalMesh(FUpperArmRArray[UKismetMathLibrary::Clamp(InHero->GUpperArmRInt, 0, FUpperArmRArray.Num() - 1)]);
		GArmLowerL->SetSkeletalMesh(FLowerArmLArray[UKismetMathLibrary::Clamp(InHero->GLowerArmLInt, 0, FLowerArmLArray.Num() - 1)]);
		GArmLowerR->SetSkeletalMesh(FLowerArmRArray[UKismetMathLibrary::Clamp(InHero->GLowerArmRInt, 0, FLowerArmRArray.Num() - 1)]);
		GHandL->SetSkeletalMesh(FHandLArray[UKismetMathLibrary::Clamp(InHero->GHandLInt, 0, FHandLArray.Num() - 1)]);
		GHandR->SetSkeletalMesh(FHandRArray[UKismetMathLibrary::Clamp(InHero->GHandRInt, 0, FHandRArray.Num() - 1)]);
		GHips->SetSkeletalMesh(FHipsArray[UKismetMathLibrary::Clamp(InHero->GHipsInt, 0, FHipsArray.Num() - 1)]);
		GLegL->SetSkeletalMesh(FLegLArray[UKismetMathLibrary::Clamp(InHero->GLegLInt, 0, FLegLArray.Num() - 1)]);
		GLegR->SetSkeletalMesh(FLegRArray[UKismetMathLibrary::Clamp(InHero->GLegRInt, 0, FLegRArray.Num() - 1)]);
	}
	switch (InHero->HeadCoveringInt) {
	case 0:
		//set AHeadCoveringNoHair's skeletal mesh to nothing
		AHeadCoveringNoHair->SetSkeletalMesh(nullptr);
		AHeadCoveringBaseHair->SetSkeletalMesh(nullptr);
		AHeadCoveringNoFacialHair->SetSkeletalMesh(nullptr);
		AHair->SetSkeletalMesh(AHairArray[UKismetMathLibrary::Clamp(InHero->AHairInt, 0, AHairArray.Num() - 1)]);
		break;
	case 1:
		AHeadCoveringNoHair->SetSkeletalMesh(AHeadCoverNoHairArray
			[UKismetMathLibrary::Clamp(InHero->AHeadCoverNoHairInt, 0, AHeadCoverNoHairArray.Num() - 1)]);
		AHair->SetSkeletalMesh(nullptr);
		break;
	case 2:
		AHeadCoveringBaseHair->SetSkeletalMesh(AHeadCoverBaseHairArray
			[UKismetMathLibrary::Clamp(InHero->AHeadCoverBaseHairInt, 0, AHeadCoverBaseHairArray.Num() - 1)]);
		AHair->SetSkeletalMesh(AHairArray[1]);
		break;
	case 3:
		AHeadCoveringNoFacialHair->SetSkeletalMesh(AHeadCoverNoFacialHairArray
			[UKismetMathLibrary::Clamp(InHero->AHeadCoverNoFacialHairInt, 0, AHeadCoverNoFacialHairArray.Num() - 1)]);
		AHair->SetSkeletalMesh(AHairArray[UKismetMathLibrary::Clamp(InHero->AHairInt, 0, AHairArray.Num() - 1)]);
		break;
	}
	AHeadAttachmentHelmet->SetSkeletalMesh(AHeadAttachmentHelmetArray
		[UKismetMathLibrary::Clamp(InHero->AHeadAttachmentInt, 0, AHeadAttachmentHelmetArray.Num() - 1)]);
	AChestAttachment->SetSkeletalMesh(AChestAttachmentArray
		[UKismetMathLibrary::Clamp(InHero->AChestAttachmentInt, 0, AChestAttachmentArray.Num() - 1)]);
	ABackAttachment->SetSkeletalMesh(ABackAttachmentArray
		[UKismetMathLibrary::Clamp(InHero->ABackAttachmentInt, 0, ABackAttachmentArray.Num() - 1)]);
	AShoulderAttachmentL->SetSkeletalMesh(AShoulderLAttachmentArray
		[UKismetMathLibrary::Clamp(InHero->AShoulderLAttachmentInt, 0, AShoulderLAttachmentArray.Num() - 1)]);
	AShoulderAttachmentR->SetSkeletalMesh(AShoulderRAttachmentArray
		[UKismetMathLibrary::Clamp(InHero->AShoulderRAttachmentInt, 0, AShoulderRAttachmentArray.Num() - 1)]);
	AElbowAttachmentL->SetSkeletalMesh(AElbowLAttachmentArray
		[UKismetMathLibrary::Clamp(InHero->AElbowLAttachmentInt, 0, AElbowLAttachmentArray.Num() - 1)]);
	AElbowAttachmentR->SetSkeletalMesh(AElbowRAttachmentArray
		[UKismetMathLibrary::Clamp(InHero->AElbowRAttachmentInt, 0, AElbowRAttachmentArray.Num() - 1)]);
	AHipAttachment->SetSkeletalMesh(AHipsAttachmentArray
		[UKismetMathLibrary::Clamp(InHero->AHipsAttachmentInt, 0, AHipsAttachmentArray.Num() - 1)]);
	AKneeAttachmentL->SetSkeletalMesh(AKneeLAttachmentArray
		[UKismetMathLibrary::Clamp(InHero->AKneeLAttachmentInt, 0, AKneeLAttachmentArray.Num() - 1)]);
	AKneeAttachmentR->SetSkeletalMesh(AKneeRAttachmentArray
		[UKismetMathLibrary::Clamp(InHero->AKneeRAttachmentInt, 0, AKneeRAttachmentArray.Num() - 1)]);
	AExtra->SetSkeletalMesh(AExtraArray
		[UKismetMathLibrary::Clamp(InHero->AExtraInt, 0, AExtraArray.Num() - 1)]);

	GTorso->SetLeaderPoseComponent(GetMesh());
	GHead->SetLeaderPoseComponent(GetMesh());
	GEyebrows->SetLeaderPoseComponent(GetMesh());
	GFacialHair->SetLeaderPoseComponent(GetMesh());
	GArmUpperL->SetLeaderPoseComponent(GetMesh());
	GArmUpperR->SetLeaderPoseComponent(GetMesh());
	GArmLowerL->SetLeaderPoseComponent(GetMesh());
	GArmLowerR->SetLeaderPoseComponent(GetMesh());
	GHandL->SetLeaderPoseComponent(GetMesh());
	GHandR->SetLeaderPoseComponent(GetMesh());
	GHips->SetLeaderPoseComponent(GetMesh());
	GLegL->SetLeaderPoseComponent(GetMesh());
	GLegR->SetLeaderPoseComponent(GetMesh());
	AHeadCoveringNoHair->SetLeaderPoseComponent(GetMesh());
	AHair->SetLeaderPoseComponent(GetMesh());
	AHeadAttachmentHair->SetLeaderPoseComponent(GetMesh());
	AHeadAttachmentHelmet->SetLeaderPoseComponent(GetMesh());
	AChestAttachment->SetLeaderPoseComponent(GetMesh());
	AHeadCoveringBaseHair->SetLeaderPoseComponent(GetMesh());
	AHeadCoveringNoFacialHair->SetLeaderPoseComponent(GetMesh());
	AShoulderAttachmentL->SetLeaderPoseComponent(GetMesh());
	AShoulderAttachmentR->SetLeaderPoseComponent(GetMesh());
	AElbowAttachmentL->SetLeaderPoseComponent(GetMesh());
	AElbowAttachmentR->SetLeaderPoseComponent(GetMesh());
	AHipAttachment->SetLeaderPoseComponent(GetMesh());
	AKneeAttachmentL->SetLeaderPoseComponent(GetMesh());
	AKneeAttachmentR->SetLeaderPoseComponent(GetMesh());
	AExtra->SetLeaderPoseComponent(GetMesh());
}


void UHeroGenerator::SetMeshesHidden(bool bIsHidden) {
	GTorso->SetHiddenInGame(bIsHidden);
	GHead->SetHiddenInGame(bIsHidden);
	GEyebrows->SetHiddenInGame(bIsHidden);
	GFacialHair->SetHiddenInGame(bIsHidden);
	GArmUpperL->SetHiddenInGame(bIsHidden);
	GArmUpperR->SetHiddenInGame(bIsHidden);
	GArmLowerL->SetHiddenInGame(bIsHidden);
	GArmLowerR->SetHiddenInGame(bIsHidden);
	GHandL->SetHiddenInGame(bIsHidden);
	GHandR->SetHiddenInGame(bIsHidden);
	GHips->SetHiddenInGame(bIsHidden);
	GLegL->SetHiddenInGame(bIsHidden);
	GLegR->SetHiddenInGame(bIsHidden);
	AHeadCoveringNoHair->SetHiddenInGame(bIsHidden);
	AHair->SetHiddenInGame(bIsHidden);
	AHeadAttachmentHair->SetHiddenInGame(bIsHidden);
	AHeadAttachmentHelmet->SetHiddenInGame(bIsHidden);
	AChestAttachment->SetHiddenInGame(bIsHidden);
	AHeadCoveringBaseHair->SetHiddenInGame(bIsHidden);
	AHeadCoveringNoFacialHair->SetHiddenInGame(bIsHidden);
	AShoulderAttachmentL->SetHiddenInGame(bIsHidden);
	AShoulderAttachmentR->SetHiddenInGame(bIsHidden);
	AElbowAttachmentL->SetHiddenInGame(bIsHidden);
	AElbowAttachmentR->SetHiddenInGame(bIsHidden);
	AHipAttachment->SetHiddenInGame(bIsHidden);
	AKneeAttachmentL->SetHiddenInGame(bIsHidden);
	AKneeAttachmentR->SetHiddenInGame(bIsHidden);
	AExtra->SetHiddenInGame(bIsHidden);
	ABackAttachment->SetHiddenInGame(bIsHidden);
}


