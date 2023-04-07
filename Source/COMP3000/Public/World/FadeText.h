// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "UI/AI/UWB_BasicText.h"
#include "FadeText.generated.h"

UCLASS()
class COMP3000_API AFadeText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFadeText();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	TObjectPtr<UWidgetComponent> TextWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	float Age;
	
	UFUNCTION()
	void DestroyText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FTimerHandle TimerHandle;

};
