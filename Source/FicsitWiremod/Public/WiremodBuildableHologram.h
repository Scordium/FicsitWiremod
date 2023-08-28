// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Configuration/Properties/ConfigPropertyFloat.h"
#include "Configuration/Properties/ConfigPropertySection.h"
#include "Hologram/FGBuildableHologram.h"
#include "Utility/WiremodGameWorldModule.h"
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
		mNeedsValidFloor = false;
		mAttachmentPointSnapDistanceThreshold = 3000;
	}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		if(UseConfigGridSize) mGridSnapSize = Cast<UConfigPropertyFloat>(Cast<UConfigPropertySection>(UWiremodGameWorldModule::Self->GetConfig())->SectionProperties["WiremodHolo_SnapSize"])->Value;
	}
	
	virtual void SetHologramLocationAndRotation(const FHitResult& hitResult) override;

	virtual bool IsValidHitResult(const FHitResult& hitResult) const override
	{
		return IsValid(hitResult.GetActor());
	};

	virtual void ScrollRotate(int32 delta, int32 step) override
	{
		mScrollRotation += delta * step;
	}

	UPROPERTY(EditDefaultsOnly)
	bool UseConfigGridSize = true;
};
