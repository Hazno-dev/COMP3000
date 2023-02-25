// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseAICharacter.h"
#include "Animation/AnimInstance.h"
#include "EBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Transient, Blueprintable, HideCategories=AnimInstance, BlueprintType)
class COMP3000_API UEBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEBaseAnimInstance();

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Movement Data")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Movement Data")
	bool IsMoving;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Movement Data")
	EAIState AIState;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Reference")
	ABaseAICharacter* BaseOwningCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Reference")
	UCharacterMovementComponent* BaseMovementComponent;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeInitializeAnimation() override;

	UFUNCTION()
	void AIStateChange();
};
