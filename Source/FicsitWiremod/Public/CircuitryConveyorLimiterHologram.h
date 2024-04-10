// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildables/FGBuildableConveyorBelt.h"
#include "Hologram/FGBuildableHologram.h"
#include "CircuitryConveyorLimiterHologram.generated.h"

UCLASS()
class FICSITWIREMOD_API ACircuitryConveyorLimiterHologram : public AFGBuildableHologram
{
	GENERATED_BODY()

public:

	virtual void SetHologramLocationAndRotation(const FHitResult& hitResult) override;

	virtual void ConfigureComponents(AFGBuildable* inBuildable) const override;

	virtual bool IsValidHitActor(AActor* hitActor) const override { return true; }

	UPROPERTY() AFGBuildableConveyorBelt* SnappedBelt;
	UPROPERTY() float BeltOffset;
};
