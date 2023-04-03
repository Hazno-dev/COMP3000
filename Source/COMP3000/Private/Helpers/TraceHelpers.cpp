// Callum Stables, 2023


#include "Helpers/TraceHelpers.h"
#include "Engine/World.h"

FVector UTraceHelpers::TraceGroundLocation(AActor* Actor, float ZOffset, float TraceDistance)
{
	FVector GroundLocation = FVector::ZeroVector;

	if (!IsValid(Actor))
		return GroundLocation;

	UWorld* WorldRef = Actor->GetWorld();

	FVector CharacterLocation = Actor->GetActorLocation();
	FVector StartTrace = CharacterLocation + FVector(0.f, 0.f, ZOffset);
	FVector EndTrace = StartTrace + FVector(0.f, 0.f, -TraceDistance);
	FHitResult HitResult;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = true;
	CollisionQueryParams.AddIgnoredActor(Actor);

	if (WorldRef->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, CollisionQueryParams))
	{
		GroundLocation = HitResult.Location;
	}

	return GroundLocation;
}
