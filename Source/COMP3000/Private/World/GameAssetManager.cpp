// Callum Stables, 2023


#include "World/GameAssetManager.h"

#include "AbilitySystemGlobals.h"

void UGameAssetManager::StartInitialLoading() {
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
	UE_LOG(LogTemp, Warning, TEXT("GameAssetManager::StartInitialLoading"));
}
