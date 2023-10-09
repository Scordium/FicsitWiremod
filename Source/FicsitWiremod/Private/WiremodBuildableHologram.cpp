// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodBuildableHologram.h"


void AWiremodBuildableHologram::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
	if(!TrySnapToActor(hitResult)) SetActorLocation(hitResult.ImpactPoint.GridSnap(GetGridSize()));

	auto ImpactNormal = hitResult.ImpactNormal;
	ImpactNormal.Normalize();
	auto ImpactPointRotation = FQuat::FindBetweenNormals(FVector::UpVector, ImpactNormal);
	auto ScrollAdjustedRotation = ImpactPointRotation * FRotator(0, mScrollRotation - 90, 0).Quaternion();
	
	SetActorRotation(ScrollAdjustedRotation);
}
