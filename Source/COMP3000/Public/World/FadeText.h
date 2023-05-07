// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
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
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	float Age;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	bool bIsWorldFadeText;
	
	UFUNCTION()
	void DestroyText();

	UFUNCTION()
	void OnOverlapBeginBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void BeginFade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FTimerHandle TimerHandle;

};
