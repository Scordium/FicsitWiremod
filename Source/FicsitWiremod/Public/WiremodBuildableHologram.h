// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Hologram/FGBuildableHologram.h"
#include "WiremodBuildableHologram.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API AWiremodBuildableHologram : public AFGBuildableHologram
{
	GENERATED_BODY()
	AWiremodBuildableHologram()
	{
		mGridSnapSize = 0;
		mNeedsValidFloor = false;
	}
	
	virtual void SetHologramLocationAndRotation(const FHitResult& hitResult) override;

	virtual bool IsValidHitResult(const FHitResult& hitResult) const override
	{
		return hitResult.Actor.IsValid();
	};

	UPROPERTY(EditDefaultsOnly)
	bool UseConfigGridSize = true;
};
