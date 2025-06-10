// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CircuitryBreadboard.h"
#include "Behaviour/CircuitryStatics.h"
#include "Hologram/FGBuildableHologram.h"
#include "WiremodBuildableHologram.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API AWiremodBuildableHologram : public AFGBuildableHologram
{
	GENERATED_BODY()
	
public:
	AWiremodBuildableHologram()
	{
		mNeedsValidFloor = false;
		mAttachmentPointSnapDistanceThreshold = 3000;
	}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		mGridSnapSize = CircuitryConfig::GetHologramGridSize();
	}
	
	virtual void SetHologramLocationAndRotation(const FHitResult& hitResult) override;

	virtual void FilterAttachmentPoints(TArray<const FFGAttachmentPoint*>& Points, class AFGBuildable* pBuildable, const FHitResult& HitResult) const override;

	virtual void ConfigureActor(class AFGBuildable* inBuildable) const override;

	virtual bool IsValidHitResult(const FHitResult& hitResult) const override
	{
		if (!BreadboardReference) return IsValid(hitResult.GetActor());

		auto Coordinates = BreadboardReference->GetClosestBoardPointCoordinates(hitResult.Location);
		return !BreadboardReference->IsCellOccupied(Coordinates);
	};

	virtual void ScrollRotate(int32 delta, int32 step) override
	{
		int32 NewRot = mScrollRotation + delta * step;
		mScrollRotation = NewRot - NewRot%step;
	}

	virtual int32 GetRotationStep() const override { return 5; }

	UFUNCTION(BlueprintNativeEvent)
	double GetGridSize();
	double GetGridSize_Implementation() { return mGridSnapSize; }

protected:
	UPROPERTY()
	ACircuitryBreadboard* BreadboardReference;
};
