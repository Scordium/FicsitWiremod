#include "Behaviour/Displays/ManagedSign/Holo/ManagedSignVanillaHologram.h"

#include "WiremodUtils.h"
#include "Behaviour/Displays/ManagedSign/ManagedSign.h"

#define PROJECT(v, a) FVector::PointPlaneProject(v, a->GetActorLocation(), a->GetActorUpVector())

void AManagedSignVanillaHologram::BeginPlay()
{
	Super::BeginPlay();

	mGridSnapSize = CircuitryConfig::GetHologramGridSize();
	
	auto Sign = Cast<AManagedSign>(GetBuildClass().GetDefaultObject());
	if(!Sign) return;

	SizeSelf = Sign->Data.Size;
	OffsetSelf = Sign->HologramOffset;
	VanillaVersion = Sign->VanillaVersion;
}

void AManagedSignVanillaHologram::SetHologramLocationAndRotation(const FHitResult& HitResult)
{
	// I spent 3 days writing this, only for it to be replaced with InverseTransformPosition()
	// I hate this code and i want nothing to do with it
	if(auto OtherSign = Cast<AManagedSign>(HitResult.GetActor()))
	{
		const FVector2D Offset = (OtherSign->Data.Size + SizeSelf)/4 - FVector2D(0.1); //A small 0.1 offset to hide any gaps between signs (very visible in billboards)
		const auto GridSz = (FVector2D::Min(OtherSign->Data.Size, SizeSelf)/8).ClampAxes(6.25, 50);
		
		//Transform into relative space to simplify calculations
		auto Rotated = OtherSign->GetTransform().InverseTransformPosition(HitResult.Location);
		
		//This checks for Left/Right of the sign.
		//When the [X] is greater than [Y] then we snap the [X] position to the sign's edge + our own size offset to not clip into it
		if(FMath::Abs(Rotated.X/OtherSign->Data.Size.X) > FMath::Abs(Rotated.Y/OtherSign->Data.Size.Y))
		{
			Rotated.X = FMath::FloatSelect(Rotated.X, Offset.X, -Offset.X);
			Rotated.Y = FMath::GridSnap(Rotated.Y, GridSz.Y);
		}
		//Does the same but for Above/Below of the sign.
		else
		{
			Rotated.X = FMath::GridSnap(Rotated.X, GridSz.X);
			Rotated.Y = FMath::FloatSelect(Rotated.Y, Offset.Y, -Offset.Y);
		}
		
		//Return from relative space to world space
		FVector ModifiedWorldPosition = OtherSign->GetTransform().Inverse().InverseTransformPosition(Rotated);
		
		//For a good measure this also projects the world position to sign's Z plane
		FVector ProjectedPosition = FVector::PointPlaneProject(ModifiedWorldPosition, OtherSign->GetActorLocation(), OtherSign->GetActorUpVector());


		FVector FinalPosition = ProjectedPosition + OtherSign->GetActorUpVector() * (OtherSign->HologramOffset - OffsetSelf);
		
		SetActorLocationAndRotation(FinalPosition, OtherSign->GetActorRotation());
	}
	else
	{
		auto ImpactQuat = FQuat::FindBetweenNormals(FVector::UpVector, HitResult.ImpactNormal);
	
		auto MagicNumber = FMath::Abs(FMath::RadiansToDegrees(FMath::Atan2(HitResult.ImpactNormal.X, HitResult.ImpactNormal.Y)));
	
		auto ScrollRotationQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(mScrollRotation + MagicNumber));
	
		SetActorLocationAndRotation(HitResult.ImpactPoint.GridSnap(mGridSnapSize), ImpactQuat * ScrollRotationQuat);
	}

	
}

bool AManagedSignVanillaHologram::TryUpgrade(const FHitResult& HitResult)
{
	auto VanillaSign = Cast<AFGBuildableWidgetSign>(HitResult.GetActor());
	if(VanillaSign && VanillaSign->IsA(VanillaVersion))
	{
		SetActorTransform(VanillaSign->GetTransform());
		AddActorWorldRotation(FRotator(0, 0, 90));
		UpgradedActor = VanillaSign;

		return true;
	}

	UpgradedActor = nullptr;
	return false;
}
