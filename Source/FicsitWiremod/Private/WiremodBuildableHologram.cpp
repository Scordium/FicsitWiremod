// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodBuildableHologram.h"


void AWiremodBuildableHologram::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
	if(!TrySnapToActor(hitResult)) SetActorLocation(hitResult.ImpactPoint.GridSnap(mGridSnapSize));

	FVector DirectionVector = FVector::UpVector;
	
	FVector RotationAxis = FVector::CrossProduct(DirectionVector, hitResult.ImpactNormal);
	RotationAxis.Normalize();
	
	double DotProduct = FVector::DotProduct(DirectionVector, hitResult.ImpactNormal);
	double RotationAngle = acosf(DotProduct);
	auto Quat = FQuat(RotationAxis, RotationAngle);
	
	FQuat NewRotQuat = Quat * FRotator(0, mScrollRotation, 0).Quaternion();
	SetActorRotation(NewRotQuat);
}
