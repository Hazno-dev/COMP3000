// Callum Stables, 2023


#include "Heroes/GAS/PlayerAbilitySystemComponent.h"

void UPlayerAbilitySystemComponent::ReceiveDamage(UPlayerAbilitySystemComponent* SourceASC, float UnmitigatedDamage,
	float MitigatedDamage) {
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
