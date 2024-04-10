
#include "CircuitryConveyorLimiterHologram.h"

#include "Behaviour/Miscellaneous/ConveyorLimiter.h"
#include "Buildables/FGBuildableConveyorBelt.h"
#include "Utility/CircuitryLogger.h"

void ACircuitryConveyorLimiterHologram::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
	auto HitActor = hitResult.GetActor();

	if(auto Conveyor = Cast<AFGBuildableConveyorBelt>(HitActor))
	{
		auto Spline = Conveyor->GetSplineComponent();
		auto ActorLocation = Spline->FindLocationClosestToWorldLocation(hitResult.Location, ESplineCoordinateSpace::World);
		auto ActorRotation = Spline->FindRotationClosestToWorldLocation(hitResult.Location, ESplineCoordinateSpace::World);
		SetActorLocationAndRotation(ActorLocation, ActorRotation);

		SnappedBelt = Conveyor;
		BeltOffset = Spline->FindDistanceClosestToWorldLocation(hitResult.Location);
		
		return;
	}
	else
	{
		SnappedBelt = nullptr;
		BeltOffset = 0;
		SetActorLocationAndRotation(hitResult.Location, FQuat(FVector::UpVector, FMath::DegreesToRadians(mScrollRotation)));	
	}
}

void ACircuitryConveyorLimiterHologram::ConfigureComponents(AFGBuildable* inBuildable) const
{
	Super::ConfigureComponents(inBuildable);

	auto Limiter = Cast<AConveyorLimiter>(inBuildable);
	if(!Limiter) return;

	if(SnappedBelt)
	{
		auto SplitBelt = AFGBuildableConveyorBelt::Split(SnappedBelt, BeltOffset, false);
		auto Input = Limiter->GetInputConnector();
		auto Output = Limiter->GetOutputConnector();

		auto Belt1 = SplitBelt[0];
		auto Belt2 = SplitBelt[1];

		if(Belt1->GetConnection0()->IsConnected()) Belt1->GetConnection1()->SetConnection(Input);
		else Belt1->GetConnection0()->SetConnection(Input);

		if(Belt2->GetConnection0()->IsConnected()) Belt2->GetConnection1()->SetConnection(Output);
		else Belt2->GetConnection0()->SetConnection(Output);
	}
}
