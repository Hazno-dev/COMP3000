// Callum Stables, 2023


#include "World/FadeText.h"

// Sets default values
AFadeText::AFadeText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	TextWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TextWidgetComponent"));
	TextWidgetComponent->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	
	bIsWorldFadeText = false;
}

// Called when the game starts or when spawned
void AFadeText::BeginPlay()
{
	Super::BeginPlay();

	if (!bIsWorldFadeText) GetWorldTimerManager().SetTimer(TimerHandle, this, &AFadeText::DestroyText, Age, false);
	else BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFadeText::OnOverlapBeginBox);
	
}

void AFadeText::DestroyText() {
	Destroy();
}

void AFadeText::OnOverlapBeginBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	BeginFade();
	
}

void AFadeText::BeginFade() {
	if (UUWB_BasicText* Text = Cast<UUWB_BasicText>(TextWidgetComponent->GetUserWidgetObject())) {
		Text->bAutoFade = true;
	}
}

