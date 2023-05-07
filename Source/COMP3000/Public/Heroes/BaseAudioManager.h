// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Sound/SoundCue.h"
#include "BaseAudioManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMP3000_API UBaseAudioManager : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseAudioManager();
	
	/* Base attenuation settings */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundAttenuation> BaseAttenuation;

	// Sounds
	/* Hurt Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundCue> HurtSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundAttenuation> HurtAttenuation;

	/* Death Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundCue> DeathSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundAttenuation> DeathAttenuation;

	/* Base attack sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundCue> BaseAttackSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundAttenuation> BaseAttackAttenuation;

	/* Walking sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TArray<USoundCue*> FootstepSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundAttenuation> FootstepAttenuation;

	/* Projectile Summon Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundCue> ProjectileSummonSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundAttenuation> ProjectileSummonAttenuation;

	/* Alerted Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundCue> AlertedSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundAttenuation> AlertedAttenuation;

	/* Scream sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundCue> ScreamSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundAttenuation> ScreamAttenuation;

	/* Level up sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundCue> LevelUpSound;

	/* Jump sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundCue> JumpSound;

	/* Hero Swap Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundCue> HeroSwapSound;
	
	
	// Functions
	UFUNCTION(BlueprintCallable)
	void PlayHurtSound(bool bIs2D = false);

	UFUNCTION(BlueprintCallable)
	void PlayDeathSound(bool bIs2D = false);

	UFUNCTION(BlueprintCallable)
	void PlayBaseAttackSound(bool bIs2D = false);

	UFUNCTION(BlueprintCallable)
	void PlayFootstepSound(bool bIs2D = false);

	UFUNCTION(BlueprintCallable)
	void PlayProjectileSummonSound(bool bIs2D = false);

	UFUNCTION(BlueprintCallable)
	void PlayAlertedSound(bool bIs2D = false);

	UFUNCTION(BlueprintCallable)
	void PlayScreamSound(bool bIs2D = false);

	UFUNCTION(BlueprintCallable)
	void PlayLevelUpSound();

	UFUNCTION(BlueprintCallable)
	void PlayJumpSound();

	UFUNCTION(BlueprintCallable)
	void PlayHeroSwapSound();
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	int32 PreviousFootstepIndex = -1;
	
};
