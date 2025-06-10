#include "CircuitryBreadboardHologram.h"

#include "CircuitryBreadboard.h"
#include "CircuitryStatics.h"
#include "VectorUtils.h"


void ACircuitryBreadboardHologram::BeginPlay()
{
	Super::BeginPlay();
	mNeedsValidFloor = false;
	mGridSnapSize = CircuitryConfig::GetHologramGridSize();
}

void ACircuitryBreadboardHologram::ConfigureActor(AFGBuildable* inBuildable) const
{
	Super::ConfigureActor(inBuildable);

	auto Breadboard = Cast<ACircuitryBreadboard>(inBuildable);
	if(!Breadboard) return;

	Breadboard->SetActorScale3D(FVector::OneVector);
	Breadboard->BoardSize = FBreadboardSize(Size/100);
}

void ACircuitryBreadboardHologram::SetHologramLocationAndRotation(const FHitResult& HitResult)
{
	const auto Location = HitResult.Location.GridSnap(mGridSnapSize);
	const auto Normal = HitResult.ImpactNormal;

	if(CurrentStep == BBPS_Build)
	{
		StartPosition = Location;
		StartImpactNormal = Normal;
	
		SetActorLocationAndRotation(StartPosition, CalculateRotation(Normal));
	}
	else
	{
		auto CurrentPosition = FVector::PointPlaneProject(Location, StartPosition, StartImpactNormal);
		auto PositionDelta = (CurrentPosition - StartPosition).GridSnap(SizeStep/2);

		auto SizeRaw = FVector2D(GetActorRotation().UnrotateVector(PositionDelta)).GetAbs() * 2;

		Size = UVectorUtils::GridSnap2D(SizeRaw.ClampAxes(SizeStep, DBL_MAX), SizeStep);
		SetActorScale3D(FVector(Size/100, 1));

		auto Center = (StartPosition*2 + PositionDelta)/2;
		
		SetActorLocationAndRotation(Center, CalculateRotation(StartImpactNormal));
	}
}