// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodBuildableHologram.h"

#include "Buildables/FGBuildableFoundation.h"
#include "Hologram/FGWallHologram.h"

void AWiremodBuildableHologram::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
	auto Normal = hitResult.ImpactNormal;
	
	FVector UpVector = FVector(0, 0, 1);
	FVector RotationAxis = FVector::CrossProduct(UpVector, Normal);
	RotationAxis.Normalize();
	
	float DotProduct = FVector::DotProduct(UpVector, Normal);
	float RotationAngle = acosf(DotProduct);
	FQuat Quat = FQuat(RotationAxis, RotationAngle);
	
	FQuat NewRotQuat = Quat * FRotator(0, GetScrollRotateValue(), 0).Quaternion();
	FVector NewPos = hitResult.ImpactPoint.GridSnap(mGridSnapSize);
	
	SetActorLocationAndRotation(NewPos, NewRotQuat);
}
