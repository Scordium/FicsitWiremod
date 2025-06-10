#include "BuildablePixelHologram.h"

#include "Pixel.h"
#include "VectorUtils.h"

void ABuildablePixelHologram::SetHologramLocationAndRotation(const FHitResult& HitResult)
{
	const auto Location = HitResult.Location.GridSnap(mGridSnapSize);
	const auto Normal = HitResult.ImpactNormal;

	if(CurrentStep == BPPS_Build)
	{
		StartPosition = Location;
		StartImpactNormal = Normal;

		if (!TrySnapToActor(HitResult))
		{
			if (auto Breadboard = Cast<ACircuitryBreadboard>(HitResult.GetActor()))
			{
				Size = FVector::OneVector;
				BreadboardReference = Breadboard;
				SetActorLocation(Breadboard->GetClosestGridPoint(HitResult.Location));
			}
			else
			{
				BreadboardReference = nullptr;
				SetActorLocationAndRotation(StartPosition, CalculateRotation(Normal));
			}
		}
	}
	else
	{
		constexpr int MinSize = 1;
		constexpr int MaxSize = 5000;

		if (IsCurrentBuildMode(ScaleMode))
		{
			auto SizeRaw = GetActorRotation().UnrotateVector(Location - StartPosition).GetAbs() / ScaleDivider;
		
			Size = UVectorUtils::ClampSize_Number(SizeRaw, MinSize, MaxSize);
			auto Center = (StartPosition + Location)/2;
		
			SetActorLocationAndRotation(Center, CalculateRotation(StartImpactNormal));
		}
		else if (IsCurrentBuildMode(Scale2DMode))
		{
			auto CurrentPosition = FVector::PointPlaneProject(Location, StartPosition, StartImpactNormal);
			auto PositionDelta = (CurrentPosition - StartPosition).GridSnap(MinSize/2);

			auto SizeRaw = GetActorRotation().UnrotateVector(PositionDelta).GetAbs() / ScaleDivider;

			Size = UVectorUtils::ClampSize_Number(SizeRaw, MinSize, MaxSize);

			auto Center = (StartPosition*2 + PositionDelta)/2;
		
			SetActorLocationAndRotation(Center, CalculateRotation(StartImpactNormal));
		}
	}
	
	SetActorScale3D(Size);
}

void ABuildablePixelHologram::ConfigureComponents(class AFGBuildable* inBuildable) const
{
	Super::ConfigureComponents(inBuildable);

	auto Pixel = Cast<ABuildablePixelBase>(inBuildable);
	if (!Pixel) return;

	if (IsCurrentBuildMode(ZoopMode))
	{
		//Idk what to add here yet
	}
	else
	{
		if (auto OtherPixel = Cast<ABuildablePixelBase>(mSnappedBuilding))
		{
			Pixel->Scale = OtherPixel->Scale;
		}
		else
		{
			Pixel->Scale = Size;
		}
	}
}

bool ABuildablePixelHologram::DoMultiStepPlacement(bool isInputFromARelease)
{
	if (CurrentStep == BPPS_Build)
	{
		if (IsValid(BreadboardReference)) return true;
		
		if (IsCurrentBuildMode(ZoopMode))
		{
			CurrentStep = BPPS_Zoop;
			return false;
		}
		else if (IsCurrentBuildMode(ScaleMode) || IsCurrentBuildMode(Scale2DMode))
		{
			if (mSnappedBuilding != nullptr) return true;
			
			CurrentStep = BPPS_Scale;
			return false;
		}
		else return true;
	}
	else return true;
}
