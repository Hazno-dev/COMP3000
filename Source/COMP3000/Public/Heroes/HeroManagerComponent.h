// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "HeroGeneration.h"
#include "InputActionValue.h"
#include "GameFramework/Actor.h"
#include "HeroManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHeroSwapped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FXPChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthChanged);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMP3000_API UHeroManagerComponent : public USceneComponent
{
	GENERATED_BODY()
    
public:    
	// Sets default values for this actor's properties
	UHeroManagerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "XP DataTable")
	UDataTable* XPLevelDataTable;

	UPROPERTY()
	int32 CurrentHeroIndex;

	void SetupReferences(class UPlayerBaseAbilities* InPlayerBaseAbilitiesRef, class UHeroGenerator* InHeroGeneratorRef, class AMainCharacter* InMainCharacterRef);
	
	void SetHeroDataArray(TArray<UHeroDataInstance*> InHeroDataArray);

	UFUNCTION()
	void SwapHero(const FInputActionValue& Value);

	UFUNCTION()
	void SwapHeroByIndex(int32 Index);

	//HP Functions
	UFUNCTION()
	void TookDamage();

	UFUNCTION()
	void TookHealing();

	UFUNCTION()
	void HeroDeath();
	
	UFUNCTION()
	int32 GetCurrentHP();

	UFUNCTION()
	int32 GetCurrentHPMax();

	//Returns -1 if no heroes are alive
	UFUNCTION()
	int32 FindAnyHealthyHeroByIndex();

	//Iframes Functions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Manager")
	float IFramesDuration = 1.2f;

	UPROPERTY(BlueprintReadOnly, Category = "Hero Manager")
	bool bHasIFrames = false;

	UFUNCTION()
	void GiveIFrames();
	
	//XP Functions
	UFUNCTION()
	void AddXP(int32 XPToAdd);

	UFUNCTION()
	void LevelUp();
	
	UFUNCTION()
	int32 GetXPForNextLevel();

	UFUNCTION()
	int32 GetCurrentXP();

	UFUNCTION()
	int32 GetCurrentLevel();

	UFUNCTION()
	int32 GetCurrentXPFloor();

	UFUNCTION()
	int32 GetCurrentXPMax();
	
	TArray<FLevelXP*> XPLevelArray;

	FHeroSwapped HeroSwappedEvent;
	FLevelUp LevelUpEvent;
	FXPChanged XPChangedEvent;
	FHealthChanged HPChangedEvent;

	//Getters
	
	UFUNCTION()
	UHeroDataInstance* GetHeroDataFromIndex(int32 Index) { return HeroDataArray[Index]; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TArray<UHeroDataInstance*> HeroDataArray;

	class UPlayerBaseAbilities* PlayerBaseAbilitiesRef;
	class UHeroGenerator* HeroGeneratorRef;
	class AMainCharacter* MainCharacterRef;

	FTimerHandle IFramesTimerHandle;

	//Asset References
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Manager Asset Refs")
	TSubclassOf<UCameraShakeBase> DamageShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Manager Asset Refs")
	TSubclassOf<UCameraShakeBase> GainXPShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Manager Asset Refs")
	TSubclassOf<UCameraShakeBase> SwapHeroShakeClass;

	
	
};