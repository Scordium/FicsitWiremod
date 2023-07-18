// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodBuildableHologram.h"

#include "WiremodUtils.h"
#include "Buildables/FGBuildableFoundation.h"
#include "Configuration/Properties/ConfigPropertyFloat.h"
#include "Configuration/Properties/ConfigPropertySection.h"
#include "Hologram/FGWallHologram.h"

#define GRID_SIZE Cast<UConfigPropertyFloat>(Cast<UConfigPropertySection>(UWiremodGameWorldModule::Self->GetConfig())->SectionProperties["WiremodHolo_SnapSize"])->Value

void AWiremodBuildableHologram::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
	FVector NewPos = hitResult.ImpactPoint.GridSnap(UseConfigGridSize ? GRID_SIZE : mGridSnapSize);
	
	FVector RotationAxis = FVector::CrossProduct(FVector::UpVector, hitResult.ImpactNormal);
	RotationAxis.Normalize();
	
	double DotProduct = FVector::DotProduct(FVector::UpVector, hitResult.ImpactNormal);
	double RotationAngle = acosf(DotProduct);
	FQuat Quat = FQuat(RotationAxis, RotationAngle);
	
	FQuat NewRotQuat = Quat * FRotator(0, GetScrollRotateValue(), 0).Quaternion();
	
	SetActorLocationAndRotation(NewPos, NewRotQuat);
}
