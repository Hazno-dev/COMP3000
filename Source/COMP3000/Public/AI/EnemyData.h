// Callum Stables, 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "EnemyData.generated.h"

UENUM(BlueprintType)
enum EEnemyType
{
	Minion,
	Elite,
	Boss
};

class ABaseAICharacter;
USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABaseAICharacter> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EEnemyType> EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxHP;

	//From 0-1
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnWeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MinionSpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABaseAICharacter> MinionClass;

	FEnemyData()
	{
		SpawnWeight = 1.0f;
		EnemyClass = nullptr;
		EnemyType = Minion;
		HP = 100;
		MaxHP = HP;
		MinionSpawnCount = 0;
		MinionClass = nullptr;
	}
	
};
