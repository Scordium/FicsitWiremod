// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodBuildableHologram.h"

#include "CircuitryBreadboard.h"
#include "FGWiremodBuildable.h"


void AWiremodBuildableHologram::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
	if(!TrySnapToActor(hitResult))
	{
		if (auto Breadboard = Cast<ACircuitryBreadboard>(hitResult.GetActor()))
		{
			BreadboardReference = Breadboard;
			SetActorLocation(Breadboard->GetClosestGridPoint(hitResult.Location));
		}
		else
		{
			BreadboardReference = nullptr;
			SetActorLocation(hitResult.ImpactPoint.GridSnap(GetGridSize()));
		}
	}
	
	auto ImpactQuat = FQuat::FindBetweenNormals(FVector::UpVector, hitResult.ImpactNormal);
	
	auto MagicNumber = FMath::Abs(FMath::RadiansToDegrees(FMath::Atan2(hitResult.ImpactNormal.X, hitResult.ImpactNormal.Y)));
	
	auto ScrollRotationQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(mScrollRotation + MagicNumber));
	
	SetActorRotation(ImpactQuat * ScrollRotationQuat);
}

void AWiremodBuildableHologram::FilterAttachmentPoints(TArray<const FFGAttachmentPoint*>& Points, class AFGBuildable* pBuildable, const FHitResult& HitResult) const
{
	Super::FilterAttachmentPoints(Points, pBuildable, HitResult);
	if (auto CircuitryBuildable = Cast<AFGWiremodBuildable>(pBuildable); CircuitryBuildable && CircuitryBuildable->BreadboardReference != nullptr)
	{
		Points.Empty();
	}
}

void AWiremodBuildableHologram::ConfigureActor(class AFGBuildable* inBuildable) const
{
	Super::ConfigureActor(inBuildable);
	if (!BreadboardReference) return;
	
	if (auto CircuitryBuildable = Cast<AFGWiremodBuildable>(inBuildable))
	{
		auto Coordinates = BreadboardReference->GetClosestBoardPointCoordinates(CircuitryBuildable->GetActorLocation());
		BreadboardReference->SetBuildableAtCell(Coordinates, CircuitryBuildable);
		CircuitryBuildable->BreadboardReference = BreadboardReference;
	}
}
