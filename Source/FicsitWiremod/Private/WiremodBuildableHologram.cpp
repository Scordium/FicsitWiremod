// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodBuildableHologram.h"

#include "Buildables/FGBuildableFoundation.h"
#include "Hologram/FGWallHologram.h"

void AWiremodBuildableHologram::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
	FVector NewPos = hitResult.ImpactPoint.GridSnap(mGridSnapSize);
	
	FVector RotationAxis = FVector::CrossProduct(FVector::UpVector, hitResult.ImpactNormal);
	RotationAxis.Normalize();
	
	float DotProduct = FVector::DotProduct(FVector::UpVector, hitResult.ImpactNormal);
	float RotationAngle = acosf(DotProduct);
	FQuat Quat = FQuat(RotationAxis, RotationAngle);
	
	FQuat NewRotQuat = Quat * FRotator(0, GetScrollRotateValue(), 0).Quaternion();
	
	SetActorLocationAndRotation(NewPos, NewRotQuat);
}
