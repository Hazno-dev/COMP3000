// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGeneration/LevelGenerator.h"

#include <string>

#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TilesSize = FVector(3000, 3000, 3000);
	CurrentResetAttempts = 0;

	//Points to seek in generation
	SeekingPoints.Add(ECardinalPoints::North, FVector2D(1, 0));
	SeekingPoints.Add(ECardinalPoints::East, FVector2D(0, 1));
	SeekingPoints.Add(ECardinalPoints::South, FVector2D(-1, 0));
	SeekingPoints.Add(ECardinalPoints::West, FVector2D(0, -1));

	bShowDebugTiles = true;

}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateLevel();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DEBUG 
	if (PlayerController != nullptr)
	{
		if (PlayerController->WasInputKeyJustPressed(EKeys::E))
		{
			CurrentResetAttempts = 0;
			ResetGeneratedLevel();
			GenerateLevel();
		}
	}

}

void ALevelGenerator::GenerateLevel()
{
	SetOffTiles(); //Disable vectors inside level generation.
	TileLayoutGeneration();
	if (!IsStartEndTooClose()  || CurrentResetAttempts > 3) {
		GenerateNewLinks();
		GenerateAdditionalTiles();
		GenerateSpecialTiles();
		SpawnTileLayout();
		//this->SetActorRotation(FRotator(0, 45, 0));
	}
	else {
		CurrentResetAttempts += 1;
		ResetGeneratedLevel();
		GenerateLevel();
	}
}

void ALevelGenerator::ResetGeneratedLevel()
{
	//UE_LOG(LogTemp, Warning, TEXT("GripState: %d"), SpawnedTiles.Num());
	for (auto SpawnedTile : SpawnedTiles)
	{
		SpawnedTile->Destroy();
	}
	SpawnedTiles.Empty();
	TileLayout.Empty();
}

//Generate dungeon layout
void ALevelGenerator::TileLayoutGeneration()
{
	//Start Tile
	FTileInfo BuildInfo;
	BuildInfo.Type = ETileType::StartTile;
	BuildInfo.Directions.Add(StartDirection, true);
	TileLayout.Add(FVector2D(0,0), BuildInfo);

	PreviousLocation = FVector2D(0,0);
	CurrentLocation = FVector2D(0,0);
	TempDirect = StartDirection;

	FTileInfo MakerInfo;
	
	//Loop as many times as ValueAmount (amount of tiles to generate)
	for (int i = 0; i < ValueAmount; i++)
	{
		//Stepback up to two times
		int random = FMath::RandRange(1, 100);
		if (random <= StepBackChance && TileLayout.Num() - OffTiles.Num() > 1) {
			UE_LOG(LogTemp, Warning, TEXT("Reversed"));
			PreviousLocation = StepBackOne(PreviousLocation);
			random = FMath::RandRange(1, 100);
			if (random <= StepBackChance) PreviousLocation = StepBackOne(PreviousLocation);
		}
		if (TileLayout.Num() - OffTiles.Num() == 1) CurrentLocation = FindNextRandomLocation(PreviousLocation, StartDirection);
		else CurrentLocation = FindNextRandomLocation(PreviousLocation, Random);
		if (CurrentLocation == PreviousLocation) CurrentLocation = Backtracker(PreviousLocation, Random);
		TileLayout.Add(CurrentLocation, MakerInfo);
		TileConnector(PreviousLocation, CurrentLocation, TempDirect);
		PreviousLocation = CurrentLocation;
	}
	
	MakerInfo.Type = ETileType::EndTile;
	if (SpawnEndFurthest) CurrentLocation = FurthestFromStart(PreviousLocation, Random);
	if (CurrentLocation == PreviousLocation || !SpawnEndFurthest) {
		CurrentLocation = FindNextRandomLocation(PreviousLocation, Random);
		if (CurrentLocation == PreviousLocation) CurrentLocation = Backtracker(PreviousLocation, Random);
	}
	TileLayout.Add(CurrentLocation, MakerInfo);
	TileConnector(PreviousLocation, CurrentLocation, TempDirect);
}

//Iterate through TileLayout and spawn each item.
void ALevelGenerator::SpawnTileLayout()
{
	for (auto Tile : TileLayout)
	{
		std::vector<UClass*> ValidTiles;
		std::vector<TEnumAsByte<EConnectionType>> ValidConnections;
		
		switch (Tile.Value.Type)
		{
			case ETileType::StartTile:
				if (LinkAssetTiles(Tile, StartTiles) != nullptr) ValidTiles.push_back(LinkAssetTiles(Tile, StartTiles));
				if (ValidTiles.size() > 0) SpawnTile(ValidTiles[FMath::RandRange(0, ValidTiles.size()-1)], GetTileWorldspace(Tile.Key), RotationAmount(Tile.Value));
				break;
			case ETileType::GenericTile:
				if (LinkAssetTiles(Tile, GenericTiles) != nullptr) ValidTiles.push_back(LinkAssetTiles(Tile, GenericTiles));
				if (ValidTiles.size() > 0) SpawnTile(ValidTiles[FMath::RandRange(0, ValidTiles.size()-1)], GetTileWorldspace(Tile.Key), RotationAmount(Tile.Value));
				break;
			case ETileType::EndTile:
				if (LinkAssetTiles(Tile, EndTiles) != nullptr) ValidTiles.push_back(LinkAssetTiles(Tile, EndTiles));
				if (ValidTiles.size() > 0) SpawnTile(ValidTiles[FMath::RandRange(0, ValidTiles.size()-1)], GetTileWorldspace(Tile.Key), RotationAmount(Tile.Value));
				DistanceBetweenVec(FVector2D(0,0), Tile.Key);
				break;
			case ETileType::AdditionalTile:
				if (LinkAssetTiles(Tile, AdditionalTiles) != nullptr) ValidTiles.push_back(LinkAssetTiles(Tile, AdditionalTiles));
				if (ValidTiles.size() > 0) SpawnTile(ValidTiles[FMath::RandRange(0, ValidTiles.size()-1)], GetTileWorldspace(Tile.Key), RotationAmount(Tile.Value));
				break;
			case ETileType::SpecialTile:
				if (LinkAssetTiles(Tile, SpecialTilesCloned) != nullptr) ValidTiles.push_back(LinkAssetTiles(Tile, SpecialTilesCloned));
				if (ValidTiles.size() > 0) {
					SpawnTile(ValidTiles[FMath::RandRange(0, ValidTiles.size()-1)], GetTileWorldspace(Tile.Key), RotationAmount(Tile.Value));
					SpecialTilesCloned.Remove(ValidTiles[FMath::RandRange(0, ValidTiles.size()-1)]);
				}
				break;
		}
	}
}

//Spawn a Tile
void ALevelGenerator::SpawnTile(UClass* TileClass, const FVector& position, int Rotation = 0)
{
	//SpawnedActor Rules
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FAttachmentTransformRules TransformInfo (EAttachmentRule::KeepRelative,
		EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative,true);
	
	ATileBase* TempTile = GetWorld()->SpawnActor<ATileBase>(TileClass, position, FRotator(0,Rotation,0), SpawnInfo);
	TempTile->AttachToActor(this, TransformInfo);
	TempTile->SetActorRotation(FRotator(0,Rotation,0));
	TempTile->SetActorHiddenInGame(!bShowDebugTiles);

	//gengine rotation parameter
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(Rotation));

	if (TempTile->LevelName != "") {
		TempTile->LevelInstance = UGameplayStatics::GetStreamingLevel(GetWorld(), TempTile->LevelName)
			->CreateInstance(FString("Tile" + FString::FromInt(SpawnedTiles.Num())));
		if (TempTile->bMultiDirectional) TempTile->LevelInstance->LevelTransform = FTransform(FRotator(0,Rotation,0), TempTile->GetActorLocation());
		else TempTile->LevelInstance->LevelTransform = FTransform(FRotator(0,0,0), TempTile->GetActorLocation());
		TempTile->LevelInstance->SetShouldBeLoaded(true);
		TempTile->LevelInstance->SetShouldBeVisible(true);
	}
	SpawnedTiles.Add(TempTile);
}

//Get a tiles vector2d, convert into worldspace location.
FVector ALevelGenerator::GetTileWorldspace(FVector2D InVec)
{
	return FVector(InVec.X * TilesSize.X, InVec.Y * TilesSize.Y, 0);
}

FVector2D ALevelGenerator::FindNextRandomLocation(FVector2D InVec, TEnumAsByte<ECardinalPoints> WantedDirection)
{
	//Locations to seek
	if (WantedDirection == ECardinalPoints::Random)
	{
		std::vector<FVector2D> ValidLocations;
		std::vector<TEnumAsByte<ECardinalPoints>> ValidDirects;
		for (auto CurrentSeek : SeekingPoints) {
			if (TileLayout.Find(InVec + CurrentSeek.Value) == nullptr) {
				ValidLocations.push_back(InVec + CurrentSeek.Value);
				ValidDirects.push_back(CurrentSeek.Key);
			}
		}
		int rand = FMath::RandRange(0, ValidLocations.size() - 1);
		if (ValidLocations.size() > 1) {
			if (!IsThisCorridor(InVec, ValidDirects[rand])) {
				CorridorsSpawned = 0;
				TempDirect = ValidDirects[rand];
				return ValidLocations[rand];
			}
			CorridorsSpawned++;
			if (CorridorsSpawned >= MaxInRowAmount + 1)
			{
				UE_LOG(LogTemp, Warning, TEXT("BrokenUpCorridors"), CorridorsSpawned);
				ValidLocations.erase(ValidLocations.begin() + rand);
				ValidDirects.erase(ValidDirects.begin() + rand);
				CorridorsSpawned = 0;
				if (ValidLocations.size() > 1)
				{
					rand = FMath::RandRange(0, ValidLocations.size() - 1);
					TempDirect = ValidDirects[rand];
					return ValidLocations[rand];
				}
			}
			TempDirect = ValidDirects[rand];
			return ValidLocations[rand];
			
		}
		if (ValidLocations.size() > 0) {
			TempDirect = ValidDirects[0];
			return ValidLocations[0];
		}
		return InVec;
	}
	if (TileLayout.Find(InVec + SeekingPoints.FindRef(WantedDirection)) == nullptr)
	{
		TempDirect = WantedDirection;
		return InVec + SeekingPoints.FindRef(WantedDirection);
	}
	return InVec;
}

FVector2D ALevelGenerator::Backtracker(FVector2D InVec, TEnumAsByte<ECardinalPoints> WantedDirection)
{
	//UE_LOG(LogTemp, Warning, TEXT("Deadend: %d"), TileLayout.Num());
	TArray<FVector2D> Tilelocations;
	TileLayout.GenerateKeyArray(Tilelocations);
	for (int i = 0; i < OffTiles.Num(); i++) Tilelocations.RemoveAt(0);
	int counter = Tilelocations.Num() - 1;
	FVector2D returnVec = InVec;
	for (counter; counter > 1; counter--)
	{
		returnVec = Tilelocations[counter];
		//UE_LOG(LogTemp, Warning, TEXT("ReturnVec: %s"), *returnVec.ToString());
		if (returnVec != FindNextRandomLocation(Tilelocations[counter], WantedDirection))
		{
			//UE_LOG(LogTemp, Warning, TEXT("ReturnVecDone: %s"), *returnVec.ToString());
			//UE_LOG(LogTemp, Warning, TEXT("Tilelocations: %s"), *Tilelocations[counter].ToString());
			PreviousLocation = Tilelocations[counter];
			return FindNextRandomLocation(Tilelocations[counter], WantedDirection);
		}
	}
	return InVec;
}

FVector2D ALevelGenerator::StepBackOne(FVector2D InVec)
{
	TArray<FVector2D> Tilelocations;
	TileLayout.GenerateKeyArray(Tilelocations);
	for (int i = 0; i < OffTiles.Num(); i++) Tilelocations.RemoveAt(0);
	if (Tilelocations.Num() > 3) if (TileLayout.FindRef(Tilelocations[Tilelocations.Num() - 2]).Type == ETileType::GenericTile)
		return Tilelocations[Tilelocations.Num() - 2];
	return InVec;
}

int ALevelGenerator::DistanceBetweenVec(FVector2D Vec1, FVector2D Vec2)
{
	 return FMath::RoundToInt(FVector2D::Distance(Vec1, Vec2));;
}

bool ALevelGenerator::IsStartEndTooClose()
{
	for (auto CurrentTile : TileLayout) {
		if (CurrentTile.Value.Type != EndTile) continue;
		UE_LOG(LogTemp, Warning, TEXT("Returned: %i"), DistanceBetweenVec(FVector2D(0,0), CurrentTile.Key));
		if (DistanceBetweenVec(FVector2D(0,0), CurrentTile.Key) <= MinStartEndDistance) return true;
	}
	return false;
}

FVector2D ALevelGenerator::FurthestFromStart(FVector2D InVec, TEnumAsByte<ECardinalPoints> Direction)
{
	TArray<FVector2D> FurthestArray;
	int CurrentFurthestDist = 0;
	FVector2D TempPreviousLocation = InVec;
	FVector2D TempCurrentLocation = TempPreviousLocation;
	
	for (auto CurrentTile : TileLayout) {
		if (CurrentTile.Value.Type != ETileType::GenericTile) continue;
		if (DistanceBetweenVec(FVector2D(0,0), CurrentTile.Key) > CurrentFurthestDist) {
			CurrentFurthestDist = DistanceBetweenVec(FVector2D(0,0), CurrentTile.Key);
			FurthestArray.Add(CurrentTile.Key);
		}
	}
	if (FurthestArray.IsEmpty()) return InVec;
	for (int i = FurthestArray.Num() - 1; i >= 0; i--) {
		TempPreviousLocation = FurthestArray[i];
		TempCurrentLocation = FindNextRandomLocation(TempPreviousLocation, Random);
		if (TempCurrentLocation != TempPreviousLocation) {
			PreviousLocation = TempPreviousLocation;
			return TempCurrentLocation;
		}
	}
	return InVec;
}

void ALevelGenerator::GenerateAdditionalTiles()
{
	if (AdditionalTilesAmount == 0) return;
	int rand;
	FVector2D TempCurrentLocation;
	FVector2D TempPreviousLocation;
	TArray<FVector2D> ValidPrevVectors;
	TArray<FVector2D> ValidCurVectors;
	TArray<TEnumAsByte<ECardinalPoints>> ValidDirects;
	
	for (auto Tile : TileLayout)
	{
		if (Tile.Value.Type == GenericTile)
		{
			TempPreviousLocation = Tile.Key;
			TempCurrentLocation = FindNextRandomLocation(TempPreviousLocation, Random);
			if (TempCurrentLocation != TempPreviousLocation)
			{
				ValidPrevVectors.Add(TempPreviousLocation);
				ValidCurVectors.Add(TempCurrentLocation);
				ValidDirects.Add(TempDirect);
			}
		}
	}
	for (int i = 0; i < AdditionalTilesAmount; i++)
	{
		if (ValidPrevVectors.IsEmpty()) break;
		FTileInfo TempAdditional;
		TempAdditional.Type = AdditionalTile;
		rand = FMath::RandRange(0, ValidPrevVectors.Num() - 1);
		TileLayout.Add(ValidCurVectors[rand], TempAdditional);
		TileConnector(ValidPrevVectors[rand], ValidCurVectors[rand], ValidDirects[rand]);
		ValidPrevVectors.RemoveAt(rand);
		ValidCurVectors.RemoveAt(rand);
		ValidDirects.RemoveAt(rand);
	}
}

void ALevelGenerator::GenerateSpecialTiles()
{
	if (SpecialTiles.Num() == 0) return;

	FTileInfo SpecialInfo;
	SpecialInfo.Type = ETileType::SpecialTile;
	for (auto CurrentSpecial : SpecialTiles)
	{
		for (auto Tile : TileLayout)
		{
			if (LinkAssetTile(Tile, CurrentSpecial) && (Tile.Value.Type == ETileType::GenericTile
				|| Tile.Value.Type == ETileType::AdditionalTile))
			{
				SpecialInfo.Directions = Tile.Value.Directions;
				TileLayout.Add(Tile.Key, SpecialInfo);
				SpecialTilesCloned.Add(CurrentSpecial);
				break;
			}
		}
	}
}

void ALevelGenerator::TileConnector(FVector2D FirstTileVec, FVector2D SecondTileVec, TEnumAsByte<ECardinalPoints> FirstCardinal)
{
	//UE_LOG(LogTemp, Warning, TEXT("GripState: %i"), FirstCardinal);
	SetConnection(FirstTileVec, FirstCardinal, true);
	SetConnection(SecondTileVec, DirectionInverter(FirstCardinal), true);
}

void ALevelGenerator::SetConnection(FVector2D InTileVec, TEnumAsByte<ECardinalPoints> ToCardinal, bool enable)
{
	if (TileLayout.Find(InTileVec) != nullptr) TileLayout.Find(InTileVec)->Directions.Add(ToCardinal, enable);
}

UClass* ALevelGenerator::LinkAssetTiles(TTuple<UE::Math::TVector2<double>, FTileInfo> InTile, TArray<UClass*> InList)
{
	TArray<bool> CurrentTileMapDirections;
	InTile.Value.Directions.GenerateValueArray(CurrentTileMapDirections);
	TEnumAsByte<EConnectionType> CurrentTileConnection = GetConnectionType(CurrentTileMapDirections);
	std::vector<UClass*> ValidTiles;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CurrentTileConnection: %i"), CurrentTileConnection));
	
	for (auto CurrentGenericTile : InList)
	{
		if (CurrentTileConnection == CurrentGenericTile->GetDefaultObject<ATileBase>()->ConnectionType) {
			if (CurrentGenericTile->GetDefaultObject<ATileBase>()->bMultiDirectional) ValidTiles.push_back(CurrentGenericTile);
			else {
				TArray<bool> CurrentTileListDirections;
				CurrentGenericTile->GetDefaultObject<ATileBase>()->TileConnections.GenerateValueArray(CurrentTileListDirections);
				for (int i = 0; i < 4; i++)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CurrentTileListDirections: %i"), CurrentTileListDirections[i]));
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CurrentTileMapDirections: %i"), CurrentTileMapDirections[i]));
					if (CurrentTileMapDirections[i] != CurrentTileListDirections[i]) break;
					if (i == 3) ValidTiles.push_back(CurrentGenericTile);
				}
			}
		}
	}
	if (ValidTiles.size() > 0) return ValidTiles[FMath::RandRange(0, ValidTiles.size() - 1)];
	return nullptr;
}

bool ALevelGenerator::LinkAssetTile(TTuple<UE::Math::TVector2<double>, FTileInfo> InTile, UClass* InClass)
{
	TArray<bool> CurrentTileMapDirections;
	TArray<bool> CurrentTileListDirections;
	InTile.Value.Directions.GenerateValueArray(CurrentTileMapDirections);
	InClass->GetDefaultObject<ATileBase>()->TileConnections.GenerateValueArray(CurrentTileListDirections);
	for (int i = 0; i < 4; i++)
	{
		if (CurrentTileMapDirections[i] != CurrentTileListDirections[i]) break;
		if (i == 3) return true;
	}
	return false;
}

TEnumAsByte<EConnectionType> ALevelGenerator::GetConnectionType(TArray<bool> Direction) {
	
	if (Direction[0] && Direction[1] && Direction[2] && Direction[3]) return EConnectionType::Fourway;
	
	if (Direction[0] && Direction[1] && Direction[2] && !Direction[3]) return EConnectionType::Threeway;
	if (Direction[0] && Direction[1] && Direction[3] && !Direction[2]) return EConnectionType::Threeway;
	if (Direction[0] && Direction[2] && Direction[3] && !Direction[1]) return EConnectionType::Threeway;
	if (Direction[1] && Direction[2] && Direction[3] && !Direction[0]) return EConnectionType::Threeway;
	
	if (Direction[0] && Direction[1] && !Direction[2] && !Direction[3]) return EConnectionType::Corner;
	if (Direction[0] && Direction[3] && !Direction[1] && !Direction[2]) return EConnectionType::Corner;
	if (Direction[2] && Direction[3] && !Direction[1] && !Direction[0]) return EConnectionType::Corner;
	if (Direction[2] && Direction[1] && !Direction[0] && !Direction[3]) return EConnectionType::Corner;
	
	if (Direction[0] && Direction[2] && !Direction[1] && !Direction[3]) return EConnectionType::Line;
	if (Direction[1] && Direction[3] && !Direction[0] && !Direction[2]) return EConnectionType::Line;

	if (Direction[0] && !Direction[1] && !Direction[2] && !Direction[3]) return EConnectionType::Single;
	if (Direction[1] && !Direction[0] && !Direction[2] && !Direction[3]) return EConnectionType::Single;
	if (Direction[2] && !Direction[0] && !Direction[1] && !Direction[3]) return EConnectionType::Single;
	if (Direction[3] && !Direction[0] && !Direction[1] && !Direction[2]) return EConnectionType::Single;
	
	return EConnectionType::NONE;
}

int ALevelGenerator::RotationAmount(FTileInfo InTile) {
	TArray<bool> CurrentTileMapDirections;
	InTile.Directions.GenerateValueArray(CurrentTileMapDirections);

	//Fourway
	if (CurrentTileMapDirections[0] && CurrentTileMapDirections[1] && CurrentTileMapDirections[2] && CurrentTileMapDirections[3]) return 0;

	//Threeway
	if (CurrentTileMapDirections[0] && CurrentTileMapDirections[1] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[2]) return 0;
	if (CurrentTileMapDirections[0] && CurrentTileMapDirections[1] && CurrentTileMapDirections[2] && !CurrentTileMapDirections[3]) return 90;
	if (CurrentTileMapDirections[1] && CurrentTileMapDirections[2] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[0]) return 180;
	if (CurrentTileMapDirections[0] && CurrentTileMapDirections[2] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[1]) return 270;
	
	//Corner
	if (CurrentTileMapDirections[0] && CurrentTileMapDirections[1] && !CurrentTileMapDirections[2] && !CurrentTileMapDirections[3]) return 0;
	if (CurrentTileMapDirections[1] && CurrentTileMapDirections[2] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[3]) return 90;
	if (CurrentTileMapDirections[2] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[0]) return 180;
	if (CurrentTileMapDirections[3] && CurrentTileMapDirections[0] && !CurrentTileMapDirections[2] && !CurrentTileMapDirections[1]) return 270;

	//Line
	if (CurrentTileMapDirections[0] && CurrentTileMapDirections[2] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[3]) return 0;
	if (CurrentTileMapDirections[1] && CurrentTileMapDirections[3] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[2]) return 90;

	//Single

	if (CurrentTileMapDirections[0] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[2] && !CurrentTileMapDirections[3]) return 0;
	if (CurrentTileMapDirections[1] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[2] && !CurrentTileMapDirections[3]) return 90;
	if (CurrentTileMapDirections[2] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[3]) return 180;
	if (CurrentTileMapDirections[3] && !CurrentTileMapDirections[0] && !CurrentTileMapDirections[1] && !CurrentTileMapDirections[2]) return 270;

	return 0;
}


void ALevelGenerator::GenerateNewLinks()
{
	int counter = 0;
	if (ExtraLinksAmount == 0) return;
	for (auto Tile : TileLayout)
	{
		if (counter >= (ExtraLinksAmount + 1)) return;
		
		std::vector<TTuple<UE::Math::TVector2<double>, FTileInfo>> ValidLocations;
		for (auto CurrentSeek : SeekingPoints) {
			if (TileLayout.Find(Tile.Key + CurrentSeek.Value) != nullptr
				&& TileLayout.Find(Tile.Key + CurrentSeek.Value)->Type == ETileType::GenericTile
				&& Tile.Value.Type == ETileType::GenericTile
				&& counter < (ExtraLinksAmount + 1)) {
				if (!AreTilesConnected(Tile.Key + CurrentSeek.Value, CurrentSeek.Key))
				{
					if (counter != 0) TileConnector(Tile.Key, Tile.Key + CurrentSeek.Value, CurrentSeek.Key);
					counter++;
				}
			}
		}
	}
}

bool ALevelGenerator::AreTilesConnected(FVector2D InTile, TEnumAsByte<ECardinalPoints> Direction)
{
	return TileLayout.Find(InTile)->Directions.FindRef(DirectionInverter(Direction));
}

TEnumAsByte<ECardinalPoints> ALevelGenerator::DirectionInverter(TEnumAsByte<ECardinalPoints> Direction)
{
	switch (Direction)
	{
	case North: return South;
	case South: return North; 
	case East: return West; 
	case West: return  East; 
	}
	return North;
}

void ALevelGenerator::SetOffTiles()
{
	if (!OffTiles.IsEmpty())
	{
		for (auto OffTile : OffTiles)
		{
			FTileInfo OffTileInfo;
			OffTileInfo.Type = ETileType::OffTile;
			TileLayout.Add(OffTile, OffTileInfo);
		}
	}
}

bool ALevelGenerator::IsThisCorridor(FVector2D InVec, TEnumAsByte<ECardinalPoints> Direction)
{
	if (TileLayout.Find(InVec)->Directions.FindRef(DirectionInverter(Direction))) return true;
	return false;
}
