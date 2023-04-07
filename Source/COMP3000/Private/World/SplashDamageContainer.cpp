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
	DamageAmount = 10;
	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));
	DamageSphere->SetupAttachment(RootComponent);
	DamageSphere->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ASplashDamageContainer::BeginPlay()
{
	Super::BeginPlay();

	DamageSphere->OnComponentBeginOverlap.AddDynamic(this, &ASplashDamageContainer::OnOverlap);

	// Set a timer to destroy the actor after its age
	GetWorldTimerManager().SetTimer(TimerHandle_DestroyAfterAge, this, &ASplashDamageContainer::DestroyContainer, Age, false);

	// Spawn the Niagara VFX if it's valid
	if (IsValid(NiagaraVFX)) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
		NiagaraVFX, GetActorLocation(), GetActorRotation(), FVector::OneVector);
	
}

void ASplashDamageContainer::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Splash Damage Container Overlap"));
	
	if (!IsValid(OtherActor)) return;
	
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor))
	{
		//MainCharacter->TakeDamage(DamageAmount, FDamageEvent(), nullptr, this);
	}
	if (ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(OtherActor))
	{
		AICharacter->ReceivedDamage(DamageAmount, nullptr);
	}
	
	
}

void ASplashDamageContainer::DestroyContainer()
{
	Destroy();
}

