#include "Behaviour/Displays/ManagedSign/Holo/ManagedSignCustomSizeHologram.h"
#include "Behaviour/CircuitryStatics.h"
#include "Behaviour/Displays/ManagedSign/ManagedSign.h"

void AManagedSignCustomSizeHologram::BeginPlay()
{
	Super::BeginPlay();
	//SetActorScale3D(FVector::ZeroVector);

	mNeedsValidFloor = false;
	mGridSnapSize = CircuitryConfig::GetHologramGridSize();
}

void AManagedSignCustomSizeHologram::ConfigureActor(AFGBuildable* inBuildable) const
{
	Super::ConfigureActor(inBuildable);

	auto Sign = Cast<AManagedSign>(inBuildable);
	if(!Sign) return;

	Sign->SetActorScale3D(FVector::OneVector);
	Sign->Data.Size = Size;
}

void AManagedSignCustomSizeHologram::SetHologramLocationAndRotation(const FHitResult& HitResult)
{
	const auto Location = HitResult.Location.GridSnap(mGridSnapSize);
	const auto Normal = HitResult.ImpactNormal;

	if(CurrentStep == MSPS_Build)
	{
		StartPosition = Location;
		StartImpactNormal = Normal;
	
		SetActorLocationAndRotation(StartPosition, CalculateRotation(Normal));
	}
	else
	{
		constexpr int MinSize = 50;
		constexpr int MaxSize = 5000;
		
		auto CurrentPosition = FVector::PointPlaneProject(Location, StartPosition, StartImpactNormal);
		auto PositionDelta = (CurrentPosition - StartPosition).GridSnap(MinSize/2);

		auto SizeRaw = FVector2D(GetActorRotation().UnrotateVector(PositionDelta)).GetAbs() * 2;

		Size = UVectorUtils::GridSnap2D(SizeRaw.ClampAxes(MinSize, MaxSize), 10);
		SetActorScale3D(FVector(Size/200, 1));

		auto Center = (StartPosition*2 + PositionDelta)/2;
		
		SetActorLocationAndRotation(Center, CalculateRotation(StartImpactNormal));
	}
}
