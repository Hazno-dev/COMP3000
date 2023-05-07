// Callum Stables, 2023


#include "World/Collectables/LootCrate.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "World/Collectables/LootData.h"
#include "World/Collectables/XPOrb.h"

// Sets default values
ALootCrate::ALootCrate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(Mesh);

	Tier1Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Tier1Light"));
	Tier1Light->SetupAttachment(Mesh);

	Tier2Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Tier2Light"));
	Tier2Light->SetupAttachment(Mesh);

	XPMin = 0;
	XPMax = 10;
	bForcedSpawn = false;
	ForcedSpawnTier = 0;
}

void ALootCrate::EnableCrate(int Tier) {
	BoxComponent->SetGenerateOverlapEvents(true);
	if (IsValid(BaseCrateMesh)) Mesh->SetStaticMesh(BaseCrateMesh);

	//Load loot table // assign xp values
	if (IsValid(LootDataTable)) LootDataTable->GetAllRows<FLootTable>(TEXT("Tier"), LootTable);
	if (LootTable.Num() > 0 && LootTable.IsValidIndex(Tier)) {
		XPMin = LootTable[Tier]->MinXP;
		XPMax = LootTable[Tier]->MaxXP;

		if (Tier == 1) {
			Tier1Light->SetVisibility(true);
			if (IsValid(Tier1CrateMesh)) Mesh->SetStaticMesh(Tier1CrateMesh);
		}
		if (Tier == 2) {
			Tier2Light->SetVisibility(true);
			if (IsValid(Tier2CrateMesh)) Mesh->SetStaticMesh(Tier2CrateMesh);
		}
	}
	
}

// Called when the game starts or when spawned
void ALootCrate::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetStaticMesh(nullptr);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetGenerateOverlapEvents(false);

	Tier1Light->SetVisibility(false);
	Tier2Light->SetVisibility(false);

	if (bForcedSpawn) EnableCrate(ForcedSpawnTier);
}

void ALootCrate::OpenLootCrate() {
	if (bIsOpen) return;
	if (!IsValid(XPOrbClass)) return;

	Tier1Light->SetVisibility(false);
	Tier2Light->SetVisibility(false);
	
	const float Angle = 50;
	const float Strength = 150;
	const float XPDeathReward = FMath::RandRange(XPMin, XPMax);

	int OrbCount = 1;
	if (XPDeathReward <= 0) return;
	if (XPDeathReward > 30) OrbCount = 2;
	if (XPDeathReward > 100) OrbCount = 3;

	const float Reward = XPDeathReward / OrbCount;

	if (IsValid(OpenCrateMesh)) Mesh->SetStaticMesh(OpenCrateMesh);
	if (IsValid(OpenVFX)) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OpenVFX, GetActorLocation(), FRotator(0, 0, 0));
	if (IsValid(OpenSound) && IsValid(OpenAttenuation)) UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation(), 0.2f, 1, 0, OpenAttenuation);
	else if (IsValid(OpenSound)) UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation(), 0.2f, 1, 0);
	
	
	for (int i = 0; i < OrbCount; i++) {
		AXPOrb* XPOrb = GetWorld()->SpawnActor<AXPOrb>(XPOrbClass, GetActorLocation() + FVector(0, 0, 150), FRotator(0, 0, 0));
		if (!IsValid(XPOrb)) continue;
		XPOrb->LaunchOrb(Strength, Angle);
		XPOrb->SetXPValue(FMath::CeilToInt32( Reward));
	}

	bIsOpen = true;
}

