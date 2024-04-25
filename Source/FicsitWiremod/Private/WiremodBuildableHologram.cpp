// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodBuildableHologram.h"


void AWiremodBuildableHologram::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
	if(!TrySnapToActor(hitResult)) SetActorLocation(hitResult.ImpactPoint.GridSnap(GetGridSize()));
	
	auto ImpactQuat = FQuat::FindBetweenNormals(FVector::UpVector, hitResult.ImpactNormal);
	
	auto MagicNumber = FMath::Abs(FMath::RadiansToDegrees(FMath::Atan2(hitResult.ImpactNormal.X, hitResult.ImpactNormal.Y)));
	//if(hitResult.ImpactNormal.X > 0) MagicNumber += 180;
	
	auto ScrollRotationQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(mScrollRotation + MagicNumber));
	
	SetActorRotation(ImpactQuat * ScrollRotationQuat);
}
