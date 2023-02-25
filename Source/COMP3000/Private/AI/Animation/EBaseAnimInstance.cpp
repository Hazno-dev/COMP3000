// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Animation/EBaseAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UEBaseAnimInstance::UEBaseAnimInstance() {

	Speed = 0.0f;
	IsMoving = false;
	AIState = EAIState::Idle;

}

void UEBaseAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	BaseOwningCharacter = Cast<ABaseAICharacter>(GetOwningActor());
	if (BaseOwningCharacter != nullptr) {
		BaseMovementComponent = BaseOwningCharacter->GetCharacterMovement();
		BaseOwningCharacter->EventChangedAIState.AddDynamic(this, &UEBaseAnimInstance::AIStateChange);
	}
	
	
}

void UEBaseAnimInstance::AIStateChange() {
	if (!IsValid(BaseOwningCharacter)) return;
	
	AIState = BaseOwningCharacter->GetAIState();

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("AIState: %d"), AIState));
}

void UEBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!IsValid(BaseOwningCharacter) || !IsValid(BaseMovementComponent)) return;
	
	Speed = UKismetMathLibrary::VSizeXY(BaseMovementComponent->Velocity);
	IsMoving = (Speed > 3.0) ? true : false;
	
}
