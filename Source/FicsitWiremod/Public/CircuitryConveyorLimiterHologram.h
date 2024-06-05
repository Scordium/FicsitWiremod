// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGConstructDisqualifier.h"
#include "Behaviour/CircuitryStatics.h"
#include "Buildables/FGBuildableConveyorBelt.h"
#include "Hologram/FGBuildableHologram.h"
#include "CircuitryConveyorLimiterHologram.generated.h"

UCLASS()
class UCLCDMustSnapToBelt : public UFGConstructDisqualifier
{
	GENERATED_BODY()

public:

	UCLCDMustSnapToBelt()
	{
		mDisqfualifyingText = NSLOCTEXT("GeneratedLocTable", "ConveyorLimiter__MustSnapToBeltDisqualifier", "Conveyor limiter must be snapped to a belt!");
	}
};

UCLASS()
class FICSITWIREMOD_API ACircuitryConveyorLimiterHologram : public AFGBuildableHologram
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		HoloSize = CircuitryConfig::GetHologramGridSize();
	}

	virtual void SetHologramLocationAndRotation(const FHitResult& hitResult) override;

	virtual void ConfigureComponents(AFGBuildable* inBuildable) const override;

	virtual bool IsValidHitActor(AActor* hitActor) const override { return hitActor && hitActor->IsA<AFGBuildableConveyorBelt>(); }
	
	UPROPERTY() AFGBuildableConveyorBelt* SnappedBelt;
	UPROPERTY() float BeltOffset;

	double HoloSize;
};
