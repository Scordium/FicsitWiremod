#include "CircuitryItemBufferHologram.h"

#include "Behaviour/Miscellaneous/ItemBuffer.h"


void ACircuitryItemBufferHologram::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
	auto HitActor = hitResult.GetActor();

	if(auto Conveyor = Cast<AFGBuildableConveyorBelt>(HitActor))
	{
		auto Spline = Conveyor->GetSplineComponent();
		auto ActorLocation = Spline->FindLocationClosestToWorldLocation(hitResult.Location.GridSnap(HoloSize), ESplineCoordinateSpace::World);
		auto ActorRotation = Spline->FindRotationClosestToWorldLocation(hitResult.Location, ESplineCoordinateSpace::World);
		SetActorLocationAndRotation(ActorLocation, ActorRotation);

		SnappedBelt = Conveyor;
		BeltOffset = Spline->FindDistanceClosestToWorldLocation(hitResult.Location);
		
		ResetConstructDisqualifiers();
	}
	else
	{
		SnappedBelt = nullptr;
		BeltOffset = 0;
		AddConstructDisqualifier(UItemBufferCDMustSnapToBelt::StaticClass());
	}

	SetActorScale3D(FVector(1, mScrollRotation%2 == 0 ? 1 : -1, 1));
}


void ACircuitryItemBufferHologram::ConfigureComponents(AFGBuildable* inBuildable) const
{
	Super::ConfigureComponents(inBuildable);

	auto ItemBuffer = Cast<AItemBuffer>(inBuildable);
	if(!ItemBuffer) return;

	if(SnappedBelt)
	{
		auto SplitBelt = AFGBuildableConveyorBelt::Split(SnappedBelt, BeltOffset, false);
		auto Input = ItemBuffer->GetInputConnector();
		auto Output = ItemBuffer->GetOutputConnector();

		auto Belt1 = SplitBelt[0];
		auto Belt2 = SplitBelt[1];

		if(Belt1->GetConnection0()->IsConnected()) Belt1->GetConnection1()->SetConnection(Input);
		else Belt1->GetConnection0()->SetConnection(Input);

		if(Belt2->GetConnection0()->IsConnected()) Belt2->GetConnection1()->SetConnection(Output);
		else Belt2->GetConnection0()->SetConnection(Output);
	}
}

