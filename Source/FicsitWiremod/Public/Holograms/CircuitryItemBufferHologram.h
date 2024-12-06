#pragma once

#include "CoreMinimal.h"
#include "FGConstructDisqualifier.h"
#include "Behaviour/CircuitryStatics.h"
#include "Behaviour/Miscellaneous/ItemBuffer.h"
#include "Buildables/FGBuildableConveyorBelt.h"
#include "Hologram/FGBuildableHologram.h"
#include "CircuitryItemBufferHologram.generated.h"

UCLASS()
class UItemBufferCDMustSnapToBelt : public UFGConstructDisqualifier
{
	GENERATED_BODY()

public:

	UItemBufferCDMustSnapToBelt()
	{
		mDisqfualifyingText = NSLOCTEXT("GeneratedLocTable", "ItemBuffer__MustSnapToBeltDisqualifier", "Item Buffer must be snapped to a belt!");
	}
};

UCLASS()
class FICSITWIREMOD_API ACircuitryItemBufferHologram : public AFGBuildableHologram
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

	virtual void ConfigureActor(class AFGBuildable* inBuildable) const override
	{
		Super::ConfigureActor(inBuildable);
		if(auto ItemBuffer = Cast<AItemBuffer>(inBuildable)) ItemBuffer->FlipMesh = mScrollRotation%2 == 1;
	}

	virtual bool IsValidHitActor(AActor* hitActor) const override { return hitActor && hitActor->IsA<AFGBuildableConveyorBelt>(); }

	virtual int32 GetRotationStep() const override { return 1; }
	
	UPROPERTY() AFGBuildableConveyorBelt* SnappedBelt;
	UPROPERTY() float BeltOffset;

	double HoloSize;
};
