// Callum Stables, 2023


#include "World/FadeText.h"

// Sets default values
AFadeText::AFadeText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TextWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TextWidgetComponent"));
	TextWidgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFadeText::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFadeText::DestroyText, Age, false);
}

void AFadeText::DestroyText() {
	Destroy();
}

