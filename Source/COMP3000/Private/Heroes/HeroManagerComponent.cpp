// Callum Stables, 2023


#include "Heroes/HeroManagerComponent.h"

#include "Helpers/DungeonGenerationV2Helpers.h"
#include "Heroes/HeroGenerator.h"
#include "Heroes/PlayerBaseAbilities.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
UHeroManagerComponent::UHeroManagerComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void UHeroManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(XPLevelDataTable)) XPLevelDataTable->GetAllRows("GetXP Level DataTable", XPLevelArray);

	FindNearestSpawnPoint();
}

void UHeroManagerComponent::SetupReferences(UPlayerBaseAbilities* InPlayerBaseAbilitiesRef,
	UHeroGenerator* InHeroGeneratorRef, AMainCharacter* InMainCharacterRef) {
	PlayerBaseAbilitiesRef = InPlayerBaseAbilitiesRef;
	HeroGeneratorRef = InHeroGeneratorRef;
	MainCharacterRef = InMainCharacterRef;
}

void UHeroManagerComponent::SetHeroDataArray(TArray<UHeroDataInstance*> InHeroDataArray) {

	HeroDataArray.Empty();
	
	for (auto HeroData : InHeroDataArray) {
		UHeroDataInstance* NewHeroData = DuplicateObject<UHeroDataInstance>(HeroData, this);
		HeroDataArray.Add(NewHeroData);
	}

	SwapHeroByIndexUnchecked(0, false);

	if (IsValid(MainCharacterRef)) {
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UHeroManagerComponent::SetupIconsFromGen, 0.4f, false);
	}
}

void UHeroManagerComponent::SetupIconsFromGen() {
	if (!IsValid(HeroGeneratorRef)) return;
	HeroGeneratorRef->SetupIcons(HeroDataArray);
	bIconsSetup = true;
}

void UHeroManagerComponent::SwapHero(const FInputActionValue& Value) {
	int32 Index = 0;
	if (Value[0] == 1.0)      Index = 0;
	else if (Value[0] == 2.0) Index = 1;
	else if (Value[0] == 3.0) Index = 2;
	else if (Value[0] == 4.0) Index = 3;

	SwapHeroByIndex(Index);
}


void UHeroManagerComponent::SwapHeroByIndex(int32 Index) {
	if (!IsValid(PlayerBaseAbilitiesRef) || !IsValid(HeroGeneratorRef)) return;

	if (PlayerBaseAbilitiesRef->IsAnyAbilityPlayingOrCasting()) return;

	if (!HeroDataArray.IsValidIndex(Index)) return;
	if (CurrentHeroIndex == Index) return;

	if (HeroDataArray[Index]->HeroStats.HP <= 0) return;

	if (IsValid(MainCharacterRef) && IsValid(SwapHeroShakeClass)) MainCharacterRef->SavedController->ClientStartCameraShake(SwapHeroShakeClass);

	CurrentHeroIndex = Index;
	HeroGeneratorRef->SetMeshes(HeroDataArray[Index]);

	if (IsValid(HeroDataArray[Index]->HeroStats.SwapFX)) UNiagaraFunctionLibrary::SpawnSystemAttached(HeroDataArray[Index]->HeroStats.SwapFX, MainCharacterRef->GetMesh(), 
"Pelvis", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

	PlayerBaseAbilitiesRef->SetDashMaxCharges(HeroDataArray[Index]->HeroStats.MaxDashCount);
	PlayerBaseAbilitiesRef->SetDashDuration(HeroDataArray[Index]->HeroStats.DashDuration);
	PlayerBaseAbilitiesRef->SetDashVFX(HeroDataArray[Index]->HeroStats.SwapFX, HeroDataArray[Index]->HeroStats.DashTrailVFX);

	PlayerBaseAbilitiesRef->SetAbility1Instance(HeroDataArray[Index]->Ability1InstanceOG);
	PlayerBaseAbilitiesRef->SetAbility2Instance(HeroDataArray[Index]->Ability2InstanceOG);
	PlayerBaseAbilitiesRef->SetAbility3Instance(HeroDataArray[Index]->Ability3InstanceOG);
	PlayerBaseAbilitiesRef->SetUltimateAbilityInstance(HeroDataArray[Index]->UltimateAbilityInstanceOG);

	if (IsValid(MainCharacterRef)) MainCharacterRef->AudioManager->PlayHeroSwapSound();
		
	PlayerBaseAbilitiesRef->RefreshAbilityIcons.Broadcast();
	HeroSwappedEvent.Broadcast();
}

void UHeroManagerComponent::SwapHeroByIndexUnchecked(int32 Index, bool bSpawnParticles) {
	if (!IsValid(PlayerBaseAbilitiesRef) || !IsValid(HeroGeneratorRef)) return;

	if (PlayerBaseAbilitiesRef->IsAnyAbilityPlayingOrCasting()) return;

	if (!HeroDataArray.IsValidIndex(Index)) return;

	if (HeroDataArray[Index]->HeroStats.HP <= 0) return;

	if (IsValid(MainCharacterRef) && IsValid(SwapHeroShakeClass)) MainCharacterRef->SavedController->ClientStartCameraShake(SwapHeroShakeClass);

	CurrentHeroIndex = Index;
	HeroGeneratorRef->SetMeshes(HeroDataArray[Index]);

	if (IsValid(HeroDataArray[Index]->HeroStats.SwapFX) && bSpawnParticles) UNiagaraFunctionLibrary::SpawnSystemAttached(HeroDataArray[Index]->HeroStats.SwapFX, MainCharacterRef->GetMesh(), 
"Pelvis", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

	PlayerBaseAbilitiesRef->SetDashMaxCharges(HeroDataArray[Index]->HeroStats.MaxDashCount);
	PlayerBaseAbilitiesRef->SetDashDuration(HeroDataArray[Index]->HeroStats.DashDuration);
	PlayerBaseAbilitiesRef->SetDashVFX(HeroDataArray[Index]->HeroStats.SwapFX, HeroDataArray[Index]->HeroStats.DashTrailVFX);

	PlayerBaseAbilitiesRef->SetAbility1Instance(HeroDataArray[Index]->Ability1InstanceOG);
	PlayerBaseAbilitiesRef->SetAbility2Instance(HeroDataArray[Index]->Ability2InstanceOG);
	PlayerBaseAbilitiesRef->SetAbility3Instance(HeroDataArray[Index]->Ability3InstanceOG);
	PlayerBaseAbilitiesRef->SetUltimateAbilityInstance(HeroDataArray[Index]->UltimateAbilityInstanceOG);

	PlayerBaseAbilitiesRef->RefreshAbilityIcons.Broadcast();
	HeroSwappedEvent.Broadcast();
}

UTexture2D* UHeroManagerComponent::GetCurrentHeroIcon() {
	if (!HeroDataArray.IsValidIndex(CurrentHeroIndex)) return nullptr;

	return HeroDataArray[CurrentHeroIndex]->Thumbnail;
}

UTexture2D* UHeroManagerComponent::GetHeroIconByIndex(int32 Index) {
	if (!HeroDataArray.IsValidIndex(Index)) return nullptr;

	return HeroDataArray[Index]->Thumbnail;
}


void UHeroManagerComponent::TookDamage() {
	if (bHasIFrames) return;

	GiveIFrames();

	if (IsValid(MainCharacterRef) && IsValid(DamageShakeClass)) MainCharacterRef->SavedController->ClientStartCameraShake(DamageShakeClass);
	
	HeroDataArray[CurrentHeroIndex]->HeroStats.HP--;
	HPChangedEvent.Broadcast();

	if (IsValid(MainCharacterRef)) MainCharacterRef->AudioManager->PlayHurtSound(true);
	
	HeroDeath(); // Check if players dead
}

void UHeroManagerComponent::KillVolumeDamage() {
	if (bHasIFrames) return;

	GiveIFrames();

	if (IsValid(MainCharacterRef) && IsValid(DamageShakeClass)) MainCharacterRef->SavedController->ClientStartCameraShake(DamageShakeClass);

	HeroDataArray[CurrentHeroIndex]->HeroStats.HP--;
	HPChangedEvent.Broadcast();

	if (IsValid(MainCharacterRef)) MainCharacterRef->AudioManager->PlayHurtSound(true);

	if (HeroDataArray[CurrentHeroIndex]->HeroStats.HP > 0) {
		RespawnHero();
		if (IsValid(HeroDataArray[CurrentHeroIndex]->HeroStats.SwapFX)) UNiagaraFunctionLibrary::SpawnSystemAttached(HeroDataArray[CurrentHeroIndex]->HeroStats.SwapFX, MainCharacterRef->GetMesh(), 
"Pelvis", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
	} else HeroDeath(); // Check if players dead
}

void UHeroManagerComponent::GiveIFrames() {

	IFramesTimerHandle.Invalidate();
	
	bHasIFrames = true;

	//callback to remove iFrames
	TWeakObjectPtr<UHeroManagerComponent> WeakThis(this);
	GetWorld()->GetTimerManager().SetTimer(IFramesTimerHandle, [WeakThis]() {
		if (WeakThis.IsValid()) WeakThis->bHasIFrames = false;
	}, IFramesDuration, false);
}

void UHeroManagerComponent::TookHealing() {
	if (HeroDataArray[CurrentHeroIndex]->HeroStats.HP >= HeroDataArray[CurrentHeroIndex]->HeroStats.MaxHP) return;
	HeroDataArray[CurrentHeroIndex]->HeroStats.HP++;
	HPChangedEvent.Broadcast();
}

void UHeroManagerComponent::HeroDeath() {
	if (HeroDataArray[CurrentHeroIndex]->HeroStats.HP > 0) return;

	if (IsValid(MainCharacterRef)) MainCharacterRef->AudioManager->PlayDeathSound(true);

	// Find a hero with more than 0 HP
	const int32 NextAvailableHeroIndex = FindAnyHealthyHeroByIndex();

	// Swap to hero with more than 0 hp
	if (NextAvailableHeroIndex != -1) {
		RespawnHero();
		SwapHeroByIndex(NextAvailableHeroIndex);
	} else {
		// No available heroes with more than 0 HP
		UGameplayStatics::OpenLevel(GetWorld(), "Defeat");
	}
	
}

void UHeroManagerComponent::AddXP(int32 XPToAdd) {
	
	HeroDataArray[CurrentHeroIndex]->HeroStats.XP += XPToAdd;
	XPChangedEvent.Broadcast();

	if (IsValid(MainCharacterRef) && IsValid(GainXPShakeClass)) MainCharacterRef->SavedController->ClientStartCameraShake(GainXPShakeClass);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("XP: %d"), HeroDataArray[CurrentHeroIndex]->HeroStats.XP));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("XPForNextLevel: %d"), GetXPForNextLevel()));
	
	if (GetXPForNextLevel() == 0) LevelUp();
	
}

void UHeroManagerComponent::LevelUp() {
	if (HeroDataArray[CurrentHeroIndex]->HeroStats.Level == 10) return;

	if (IsValid(MainCharacterRef)) MainCharacterRef->AudioManager->PlayLevelUpSound();
	HeroDataArray[CurrentHeroIndex]->HeroStats.Level++;
	LevelUpEvent.Broadcast();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("LevelUp")));

	if (HeroDataArray[CurrentHeroIndex]->HeroStats.ValidAbilities.Num() > 0) {

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ValidAbilities")));
		
		int RandomIndex = FMath::RandRange(0, HeroDataArray[CurrentHeroIndex]->HeroStats.ValidAbilities.Num() - 1);
		AAbilityBase* NewAbilty = NewObject<AAbilityBase>(this, HeroDataArray[CurrentHeroIndex]->HeroStats.ValidAbilities[RandomIndex]);
		NewAbilty->CustomBeginPlay(MainCharacterRef, GetWorld());
		
		if (HeroDataArray[CurrentHeroIndex]->Ability1InstanceOG == nullptr) HeroDataArray[CurrentHeroIndex]->Ability1InstanceOG = NewAbilty;
		else if (HeroDataArray[CurrentHeroIndex]->Ability2InstanceOG == nullptr) HeroDataArray[CurrentHeroIndex]->Ability2InstanceOG = NewAbilty;
		else if (HeroDataArray[CurrentHeroIndex]->Ability3InstanceOG == nullptr) HeroDataArray[CurrentHeroIndex]->Ability3InstanceOG = NewAbilty;
		else if (HeroDataArray[CurrentHeroIndex]->UltimateAbilityInstanceOG == nullptr) HeroDataArray[CurrentHeroIndex]->UltimateAbilityInstanceOG = NewAbilty;

		PlayerBaseAbilitiesRef->SetAbility1Instance(HeroDataArray[CurrentHeroIndex]->Ability1InstanceOG);
		PlayerBaseAbilitiesRef->SetAbility2Instance(HeroDataArray[CurrentHeroIndex]->Ability2InstanceOG);
		PlayerBaseAbilitiesRef->SetAbility3Instance(HeroDataArray[CurrentHeroIndex]->Ability3InstanceOG);
		PlayerBaseAbilitiesRef->SetUltimateAbilityInstance(HeroDataArray[CurrentHeroIndex]->UltimateAbilityInstanceOG);
		
		PlayerBaseAbilitiesRef->RefreshAbilityIcons.Broadcast();

		HeroDataArray[CurrentHeroIndex]->HeroStats.ValidAbilities.RemoveAt(RandomIndex);
	}
}

int32 UHeroManagerComponent::GetXPForNextLevel() {
	if (XPLevelArray.IsEmpty()) return -1;

	int32 CurrentXP = HeroDataArray[CurrentHeroIndex]->HeroStats.XP;
	int32 CurrentLevel = HeroDataArray[CurrentHeroIndex]->HeroStats.Level;

	if (CurrentLevel == 10) return -1;

	int32 TotalXPToReachCurrentLevel = 0;
	int32 XPToNextLevel = -1;

	// Loop through XPLevelArray and add up all the XPToNextLevel until we reach the current level
	for (auto XPLevel : XPLevelArray) {
		if (XPLevel->Level < CurrentLevel) {
			TotalXPToReachCurrentLevel += XPLevel->XPToNextLevel;
		} else if (XPLevel->Level == CurrentLevel) {
			XPToNextLevel = XPLevel->XPToNextLevel;
			break;
		}
	}

	int32 RemainingXPToNextLevel = (TotalXPToReachCurrentLevel + XPToNextLevel) - CurrentXP;

	if (RemainingXPToNextLevel <= 0) return 0;

	return RemainingXPToNextLevel;
}


int32 UHeroManagerComponent::GetCurrentXP() {
	if (!HeroDataArray.IsValidIndex(CurrentHeroIndex)) return -1;
	if (XPLevelArray.IsEmpty()) return -1;
	
	return HeroDataArray[CurrentHeroIndex]->HeroStats.XP;
}

int32 UHeroManagerComponent::GetCurrentLevel() {
	if (!HeroDataArray.IsValidIndex(CurrentHeroIndex)) return -1;
	if (XPLevelArray.IsEmpty()) return -1;

	return HeroDataArray[CurrentHeroIndex]->HeroStats.Level;
}

int32 UHeroManagerComponent::GetCurrentXPFloor() {
	if (!HeroDataArray.IsValidIndex(CurrentHeroIndex)) return -1;
	if (XPLevelArray.IsEmpty()) return -1;

	int32 MinXP = 0;
	for (auto XPLevel : XPLevelArray) {
		if (XPLevel->Level == HeroDataArray[CurrentHeroIndex]->HeroStats.Level) break;
		MinXP += XPLevel->XPToNextLevel;
	}

	return MinXP;
		
	
}

int32 UHeroManagerComponent::GetCurrentXPMax() {
	if (!HeroDataArray.IsValidIndex(CurrentHeroIndex)) return -1;
	if (XPLevelArray.IsEmpty()) return -1;

	int32 MaxXP = 0;
	for (auto XPLevel : XPLevelArray) {
		if (XPLevel->Level == HeroDataArray[CurrentHeroIndex]->HeroStats.Level) {
			MaxXP += XPLevel->XPToNextLevel;
			break;
		}
		MaxXP += XPLevel->XPToNextLevel;
	}

	return MaxXP;
}

void UHeroManagerComponent::RespawnHero() {
	if (!IsValid(MainCharacterRef)) return;
	if (!IsValid(PlayerSpawnPoint)) FindNearestSpawnPoint();
	if (!IsValid(PlayerSpawnPoint)) return;
	
	MainCharacterRef->SetActorLocation(PlayerSpawnPoint->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
}

void UHeroManagerComponent::FindNearestSpawnPoint() {
	if (!IsValid(GetWorld())) return;
	if (!IsValid(MainCharacterRef)) return;
	
	TArray<APSpawnPoint*> SpawnPointsArray;
	UDungeonGenerationV2Helpers::GetSpawnPointsInWorld(GetWorld(), SpawnPointsArray);

	if (SpawnPointsArray.Num() == 0) return;

	FVector PlayerLocation = MainCharacterRef->GetActorLocation();
	APSpawnPoint* NearestSpawnPoint = nullptr;
	float MinDistanceSquared = MAX_flt;

	for (APSpawnPoint* SPoint : SpawnPointsArray)
	{
		float DistanceSquared = FVector::DistSquared(PlayerLocation, SPoint->GetActorLocation());
		if (DistanceSquared < MinDistanceSquared)
		{
			MinDistanceSquared = DistanceSquared;
			NearestSpawnPoint = SPoint;
		}
	}

	if (IsValid(NearestSpawnPoint)) SetSpawnPoint(NearestSpawnPoint);
}

int32 UHeroManagerComponent::GetCurrentHP() {
	if (!HeroDataArray.IsValidIndex(CurrentHeroIndex)) return -1;

	return HeroDataArray[CurrentHeroIndex]->HeroStats.HP;
}

int32 UHeroManagerComponent::GetCurrentHPMax() {
	if (!HeroDataArray.IsValidIndex(CurrentHeroIndex)) return -1;

	return HeroDataArray[CurrentHeroIndex]->HeroStats.MaxHP;
}

int32 UHeroManagerComponent::FindAnyHealthyHeroByIndex() {
	for (int i = 0; i < HeroDataArray.Num(); i++) {
		if (HeroDataArray[i]->HeroStats.HP > 0) return i;
	}

	return -1;
}


