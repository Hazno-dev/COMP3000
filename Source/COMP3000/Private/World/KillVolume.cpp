// Callum Stables, 2023


#include "World/KillVolume.h"

#include "MainCharacter.h"
#include "AI/BaseAICharacter.h"

// Sets default values
AKillVolume::AKillVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	KillVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("KillVolume"));

}

// Called when the game starts or when spawned
void AKillVolume::BeginPlay()
{
	Super::BeginPlay();

	KillVolume->OnComponentBeginOverlap.AddDynamic(this, &AKillVolume::OnOverlapBegin);
	
}

void AKillVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (AMainCharacter* Player = Cast<AMainCharacter>(OtherActor))
	{
		Player->TookKillVolumeDamage();
	}
	if (ABaseAICharacter* Enemy = Cast<ABaseAICharacter>(OtherActor))
	{
		Enemy->Ragdoll(2);

		//Lambda delegate to destroy enemy, and test validity
		FTimerHandle DestroyHandle;
		FTimerDelegate DestroyDelegate = FTimerDelegate::CreateLambda([Enemy]()
		{
			if (IsValid(Enemy)) Enemy->ReceivedDamage(1000000, nullptr);
		});
		GetWorld()->GetTimerManager().SetTimer(DestroyHandle, DestroyDelegate, 2.0f, false);
	}
}




