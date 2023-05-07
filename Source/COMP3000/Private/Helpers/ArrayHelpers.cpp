// Callum Stables, 2023


#include "Helpers/ArrayHelpers.h"


template<typename T>
void UArrayHelpers::ShuffleArray(TArray<T>& Array, FRandomStream& SeedStream) {
	const int32 ArraySize = Array.Num() - 1;

	for (int32 i = 0; i < ArraySize; i++) {
		const int32 RandomIndex = SeedStream.RandRange(i, ArraySize);
		if (RandomIndex != i) Array.Swap(i, RandomIndex);
	}
}

void UArrayHelpers::ShuffleArrayOfLootCrates(TArray<ALootCrate*>& Array, FRandomStream& SeedStream) {
	ShuffleArray(Array, SeedStream);
}

void UArrayHelpers::ShuffleArrayOfPrefabinators(TArray<APrefabinator*>& Array, FRandomStream& SeedStream)
{
	ShuffleArray(Array, SeedStream);
}

void UArrayHelpers::ShuffleArrayOfEnemyData(TArray<FEnemyData>& Array, FRandomStream& SeedStream) {
	ShuffleArray(Array, SeedStream);
}
