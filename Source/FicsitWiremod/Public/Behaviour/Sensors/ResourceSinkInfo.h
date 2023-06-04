// 

#pragma once

#include "CoreMinimal.h"
#include "FGResourceSinkSubsystem.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ResourceSinkInfo.generated.h"

UCLASS()
class FICSITWIREMOD_API AResourceSinkInfo : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	UFUNCTION()
	int GetAvailableCoupons(){ return AFGResourceSinkSubsystem::Get(this)->GetNumCoupons(); }
	
	UFUNCTION()
	FString GetTotalPoints()
	{
		return UFGBlueprintFunctionLibrary::Conv_IntToString(
			AFGResourceSinkSubsystem::Get(this)->GetNumTotalPoints(EResourceSinkTrack::RST_Default));
	}

	UFUNCTION()
	int GetPointsTillNextCoupon() { return AFGResourceSinkSubsystem::Get(this)->GetNumPointsToNextCoupon(EResourceSinkTrack::RST_Default); }

	UFUNCTION()
	float GetProgress() { return AFGResourceSinkSubsystem::Get(this)->GetProgressionTowardsNextCoupon(EResourceSinkTrack::RST_Default); }

	UFUNCTION()
	TArray<float> GetPointsGraph()
	{
		auto Graph = AFGResourceSinkSubsystem::Get(this)->GetGlobalPointHistory(EResourceSinkTrack::RST_Default);
		TArray<float> Out;

		for (int32 Element : Graph)
			Out.Add(Element);

		return Out;
	}

	UFUNCTION()
	int GetPPM(){ return AFGResourceSinkSubsystem::Get(this)->GetGlobalPointHistory(EResourceSinkTrack::RST_Default).Last(); }



	UFUNCTION()
	FString GetTotalPoints_Exploration()
	{
		return UFGBlueprintFunctionLibrary::Conv_IntToString(
			AFGResourceSinkSubsystem::Get(this)->GetNumTotalPoints(EResourceSinkTrack::RST_Exploration));
	}

	UFUNCTION()
	int GetPointsTillNextCoupon_Exploration() { return AFGResourceSinkSubsystem::Get(this)->GetNumPointsToNextCoupon(EResourceSinkTrack::RST_Exploration); }

	UFUNCTION()
	float GetProgress_Exploration() { return AFGResourceSinkSubsystem::Get(this)->GetProgressionTowardsNextCoupon(EResourceSinkTrack::RST_Exploration); }

	UFUNCTION()
	TArray<float> GetPointsGraph_Exploration()
	{
		auto Graph = AFGResourceSinkSubsystem::Get(this)->GetGlobalPointHistory(EResourceSinkTrack::RST_Exploration);
		TArray<float> Out;

		for (int32 Element : Graph)
			Out.Add(Element);

		return Out;
	}

	UFUNCTION()
	int GetPPM_Exploration(){ return AFGResourceSinkSubsystem::Get(this)->GetGlobalPointHistory(EResourceSinkTrack::RST_Exploration).Last(); }
	
};
