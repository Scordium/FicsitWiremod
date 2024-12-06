// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGBuildableHologram.h"
#include "WiremodBuildableHologram.h"

#include "BuildablePixelHologram.generated.h"

UENUM(BlueprintType)
enum EBuildablePixelPlacementStep
{
	BPPS_Build,
	BPPS_Scale,
	BPPS_Zoop
};

UCLASS()
class FICSITWIREMOD_API ABuildablePixelHologram : public AWiremodBuildableHologram
{
	GENERATED_BODY()

public:
	virtual void SetHologramLocationAndRotation(const FHitResult& hitResult) override;

	virtual void ConfigureComponents(class AFGBuildable* inBuildable) const override;

	virtual bool IsValidHitResult(const FHitResult& hitResult) const override { return hitResult.GetActor() != nullptr; }

	virtual bool IsValidHitActor(AActor* hitActor) const override { return true; }

	virtual bool DoMultiStepPlacement(bool isInputFromARelease) override;

	FQuat CalculateRotation(const FVector& ImpactNormal) const
	{
		auto ImpactQuat = FQuat::FindBetweenNormals(FVector::UpVector, ImpactNormal);
		auto MagicNumber = FMath::Abs(FMath::RadiansToDegrees(FMath::Atan2(ImpactNormal.X, ImpactNormal.Y)));
		auto ScrollRotationQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(mScrollRotation + MagicNumber));

		return ImpactQuat * ScrollRotationQuat;
	}
	
	UPROPERTY() FVector StartPosition;
	UPROPERTY() FVector StartImpactNormal;
	UPROPERTY(BlueprintReadOnly) FVector Size = FVector::OneVector;
	UPROPERTY(BlueprintReadOnly) TEnumAsByte<EBuildablePixelPlacementStep> CurrentStep = BPPS_Build;

	UPROPERTY(EditDefaultsOnly)
	double ScaleDivider = 75;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFGHologramBuildModeDescriptor> ZoopMode;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFGHologramBuildModeDescriptor> ScaleMode;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFGHologramBuildModeDescriptor> Scale2DMode;
};
