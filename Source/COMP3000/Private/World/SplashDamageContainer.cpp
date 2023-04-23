// Callum Stables, 2023


#include "World/SplashDamageContainer.h"

#include "MainCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "AI/BaseAICharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASplashDamageContainer::ASplashDamageContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Age = 1;
	BurstDamageAmount = 10;
	DOTDamageAmount = 1;
	DOTTickRate = .25f;
	bBurstInitialDamageTimed = true;
	BurstInitialDamageTime = .25f;
	bInitialDamageTimeOver = false;
	
	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));
	DamageSphere->SetupAttachment(RootComponent);
	DamageSphere->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ASplashDamageContainer::BeginPlay()
{
	Super::BeginPlay();

	DamageSphere->OnComponentBeginOverlap.AddDynamic(this, &ASplashDamageContainer::OnOverlap);
	DamageSphere->OnComponentEndOverlap.AddDynamic(this, &ASplashDamageContainer::OnOverlapExit);

	// Set a timer to destroy the actor after its age
	GetWorldTimerManager().SetTimer(TimerHandle_DestroyAfterAge, this, &ASplashDamageContainer::DestroyContainer, Age, false);

	// Spawn the Niagara VFX if it's valid
	if (IsValid(NiagaraVFX)) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
		NiagaraVFX, GetActorLocation(), GetActorRotation(), FVector::OneVector);

	// DOT damage loop
	if (bDOT) GetWorldTimerManager().SetTimer(TimerHandle_DOTDamage, this, &ASplashDamageContainer::DOTDamage, DOTTickRate, true);
	if (bBurstInitialDamageTimed) GetWorldTimerManager().SetTimer(TimerHandle_BurstInitialDamage, this, &ASplashDamageContainer::BurstInitialDamageDisable, BurstInitialDamageTime, false);
	
}

void ASplashDamageContainer::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!IsValid(OtherActor)) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap"));

	OverlappingActors.Add(OtherActor);
	
	if (bInitialDamageTimeOver && bBurstInitialDamageTimed) return;
	
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor))
	{
		//MainCharacter->TakeDamage(DamageAmount, FDamageEvent(), nullptr, this);
	}
	if (ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(OtherActor))
	{
		if (!DamageBurstAppliedActors.Contains(AICharacter)) {
			DamageBurstAppliedActors.Add(AICharacter);
			
			if (AMainCharacter* OwnerCharacter = Cast<AMainCharacter>(GetOwner())) {
				if (IsValid(OwnerCharacter)) AICharacter->ReceivedDamage(BurstDamageAmount, OwnerCharacter);
			}
			else AICharacter->ReceivedDamage(BurstDamageAmount, nullptr);
		}
	}
	
}

void ASplashDamageContainer::OnOverlapExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (!IsValid(OtherActor)) return;

	OverlappingActors.Remove(OtherActor);
}

void ASplashDamageContainer::DOTDamage() {
	if (OverlappingActors.Num() == 0) return;
	if (!bDOT) return;
	
	for (AActor* Actor : OverlappingActors)
	{
		if (!IsValid(Actor)) continue;
		
		if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(Actor))
		{
			//MainCharacter->TakeDamage(DamageAmount, FDamageEvent(), nullptr, this);
		}
		if (ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(Actor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Dealt DOT Damage"));
			if (AMainCharacter* OwnerCharacter = Cast<AMainCharacter>(GetOwner())) {
				if (IsValid(OwnerCharacter)) AICharacter->ReceivedDamage(DOTDamageAmount, OwnerCharacter);
			}
			else AICharacter->ReceivedDamage(DOTDamageAmount, nullptr);
		}
	}
}

void ASplashDamageContainer::DestroyContainer()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DestroyAfterAge);
	GetWorldTimerManager().ClearTimer(TimerHandle_DOTDamage);
	GetWorldTimerManager().ClearTimer(TimerHandle_BurstInitialDamage);
	Destroy();
}

