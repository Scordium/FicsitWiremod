// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hologram/FGBuildableHologram.h"
#include "ManagedSignCustomSizeHologram.generated.h"

UENUM(Blueprintable, BlueprintType)
enum EManagedSignHologramPlaceStep
{
	MSPS_Build,
	MSPS_Scale
};

UCLASS()
class FICSITWIREMOD_API AManagedSignCustomSizeHologram : public AFGBuildableHologram
{
	GENERATED_BODY()

public:

	virtual void SetHologramLocationAndRotation(const FHitResult& hitResult) override;

	virtual void BeginPlay() override;

	virtual bool IsValidHitActor(AActor* hitActor) const override { return hitActor != nullptr; }

	virtual bool IsValidHitResult(const FHitResult& hitResult) const override { return hitResult.GetActor() != nullptr; }

	virtual bool DoMultiStepPlacement(bool isInputFromARelease) override
	{
		if(CurrentStep == MSPS_Build)
		{
			CurrentStep = MSPS_Scale;
			return false;
		}
		else return true;
	}

	virtual bool CanTakeNextBuildStep() const override { return true; }

	virtual void ConfigureActor(AFGBuildable* inBuildable) const override;

	virtual int32 GetRotationStep() const override { return 5; }

	FQuat CalculateRotation(const FVector& ImpactNormal) const
	{
		auto ImpactQuat = FQuat::FindBetweenNormals(FVector::UpVector, ImpactNormal);
		auto MagicNumber = FMath::Abs(FMath::RadiansToDegrees(FMath::Atan2(ImpactNormal.X, ImpactNormal.Y)));
		auto ScrollRotationQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(mScrollRotation + MagicNumber));

		return ImpactQuat * ScrollRotationQuat;
	}
	
	UPROPERTY() FVector StartPosition;
	UPROPERTY() FVector StartImpactNormal;
	UPROPERTY(BlueprintReadOnly) FVector2D Size;
	UPROPERTY(BlueprintReadOnly) TEnumAsByte<EManagedSignHologramPlaceStep> CurrentStep;
};
