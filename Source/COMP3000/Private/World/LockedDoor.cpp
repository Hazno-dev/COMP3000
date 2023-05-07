// Callum Stables, 2023


#include "World/LockedDoor.h"

#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ALockedDoor::ALockedDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	DoorMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh1"));
	DoorMesh1->SetupAttachment(RootComponent);

	DoorMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh2"));
	DoorMesh2->SetupAttachment(RootComponent);

	DoorCollisionFront = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollisionFront"));
	DoorCollisionFront->SetupAttachment(RootComponent);

	DoorCollisionBack = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollisionBack"));
	DoorCollisionBack->SetupAttachment(RootComponent);
	
	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));
	
	bIsClosed = true;
	bIsOneShot = true;
	
}

// Called when the game starts or when spawned
void ALockedDoor::BeginPlay()
{
	Super::BeginPlay();

	DoorStartLocation = GetActorLocation();
	
	if (bIsOpenDefault) OpenDoor();

	if (IsValid(DoorCurve))
	{
		FOnTimelineFloat TimelineUpdate;
		TimelineUpdate.BindUFunction(this, FName("ControlDoor"));
		DoorTimeline->AddInterpFloat(DoorCurve, TimelineUpdate);
		DoorTimeline->SetPlayRate(1.0f / 0.4f);

		FOnTimelineEvent DoorOpened;
		DoorOpened.BindUFunction(this, FName("OnDoorFinishedOpening"));
		FOnTimelineEvent DoorClosed;
		DoorClosed.BindUFunction(this, FName("OnDoorFinishedClosing"));

		DoorTimeline->SetTimelineFinishedFunc(bIsClosing ? DoorClosed : DoorOpened);
	}

	
	if (bWaveDoor) return;
	if (IsValid(DoorCollisionFront)) DoorCollisionFront->OnComponentBeginOverlap.AddDynamic(this, &ALockedDoor::OnOverlapBegin);
	if (IsValid(DoorCollisionBack)) DoorCollisionBack->OnComponentBeginOverlap.AddDynamic(this, &ALockedDoor::OnOverlapBegin);
	
}

void ALockedDoor::CloseDoor()
{
	DoorCollisionBack->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DoorCollisionFront->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FOnTimelineEvent DoorClosed;
	DoorClosed.BindUFunction(this, FName("DoorFinishedClosing"));
	DoorTimeline->SetTimelineFinishedFunc(DoorClosed);
	DoorTimeline->PlayFromStart();
}

void ALockedDoor::ControlDoor(float Value) {
	FVector TargetLocation = FVector(0.0f, 0.0f, -800.0f);

	FVector NewLocation;
	if (bIsClosing)
	{
		NewLocation = FMath::Lerp(DoorStartLocation, DoorStartLocation + TargetLocation, Value);
	}
	else
	{
		NewLocation = FMath::Lerp( DoorStartLocation + TargetLocation, DoorStartLocation, Value);
	}

	this->SetActorLocation(NewLocation);
}

void ALockedDoor::DoorFinishedClosing() {
	bIsClosed = true;
}

void ALockedDoor::OpenDoor()
{
	DoorCollisionBack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DoorCollisionFront->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (DoorOpenSound && DoorOpenAttenuation) UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation(), 0.5f, 1.0f, 0.0f, DoorOpenAttenuation);
	else if (DoorOpenSound) UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation(), 0.5f);
	
	bIsClosing = false;
	FOnTimelineEvent DoorOpened;
	DoorOpened.BindUFunction(this, FName("DoorFinishedOpening"));
	DoorTimeline->SetTimelineFinishedFunc(DoorOpened);
	DoorTimeline->PlayFromStart();
}

void ALockedDoor::DoorFinishedOpening() {
	bIsClosed = false;
	if (bIsOneShot || bKeyDoor) {
		if (IsValid(DoorCollisionFront)) DoorCollisionFront->OnComponentBeginOverlap.RemoveDynamic(this, &ALockedDoor::OnOverlapBegin);
		if (IsValid(DoorCollisionBack)) DoorCollisionBack->OnComponentBeginOverlap.RemoveDynamic(this, &ALockedDoor::OnOverlapBegin);
		
		Destroy();
	}
}

void ALockedDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (!IsValid(OtherActor)) return;

	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor)) {
		if (!MainCharacter->bHasKeyCrystal && bKeyDoor) return;

		if (bIsClosed) OpenDoor();
		if (!bIsClosed) CloseDoor();
		

	} 
	
}
