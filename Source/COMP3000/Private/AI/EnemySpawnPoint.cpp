// Callum Stables, 2023


#include "AI/EnemySpawnPoint.h"

#include "AI/BaseAICharacter.h"
#include "AI/EnemyData.h"
#include "Helpers/DungeonGenerationV2Helpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawnPoint::AEnemySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EnemyType = EEnemyType::Minion;
	bHasSpawned = false;
	bTryFindNearestGroupManager = true;
	bIsRegistered = false;
	bSecondPass = false;
}

// Called when the game starts or when spawned
void AEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawnPoint::SpawnEnemy(FEnemyData& EnemyData) {
	if (bHasSpawned) return;
	//if (EnemyData.EnemyType != EnemyType) return;
	if (!IsValid(EnemyData.EnemyClass)) return;
	
	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	ABaseAICharacter* SpawnableEnemy = NewObject<ABaseAICharacter>(this, EnemyData.EnemyClass);
	if (!IsValid(SpawnableEnemy)) return;

	// TODO: EnemyHP level scaling
	
	SpawnableEnemy->Health = EnemyData.HP;
	SpawnableEnemy->MaxHealth = EnemyData.MaxHP;

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Enemy Spawned: %s"), *SpawnableEnemy->GetName()));

	TArray<AAIGroupManager*> GroupManagers;
	int32 ClosestGroupManagerIndex = -1;
	
	//Try find a nearby group manager to assign to the enemy
	if (bTryFindNearestGroupManager) {
		UDungeonGenerationV2Helpers::GetGroupManagersInLevel(World, GroupManagers);
		
		float ClosestGroupManagerDis = FLT_MAX;
		for (int32 i = 0; i < GroupManagers.Num(); i++) {
			if (!GroupManagers.IsValidIndex(i)) continue;
			
			float Dis = FVector::Dist(GroupManagers[i]->GetActorLocation(), GetActorLocation());
			if (Dis < ClosestGroupManagerDis) {
				ClosestGroupManagerDis = Dis;
				ClosestGroupManagerIndex = i;
			}
		}
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	ABaseAICharacter* SpawnedAI = World->SpawnActor<ABaseAICharacter>(SpawnableEnemy->GetClass(), GetActorLocation(), GetActorRotation(), SpawnParams);
	if (!IsValid(SpawnedAI)) return;

	bHasSpawned = true;

	if (bTryFindNearestGroupManager)
		if (GroupManagers.IsValidIndex(ClosestGroupManagerIndex)) SpawnedAI->SetGroupManager(GroupManagers[ClosestGroupManagerIndex]);

	SpawnedAI->DeactivateAI();
}

