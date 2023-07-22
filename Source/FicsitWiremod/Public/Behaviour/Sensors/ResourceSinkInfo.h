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
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		SubsystemCache = AFGResourceSinkSubsystem::Get(this);
	}

	UFUNCTION()
	int GetAvailableCoupons()
	{
		if(!SubsystemCache) return 0;
		return SubsystemCache->GetNumCoupons();
	}
	
	UFUNCTION()
	FString GetTotalPoints()
	{
		if(!SubsystemCache) return "";
		return FString::FromInt(SubsystemCache->GetNumTotalPoints(EResourceSinkTrack::RST_Default));
	}

	UFUNCTION()
	int GetPointsTillNextCoupon()
	{
		if(!SubsystemCache) return 0;
		return SubsystemCache->GetNumPointsToNextCoupon(EResourceSinkTrack::RST_Default);
	}

	UFUNCTION()
	double GetProgress()
	{
		if(!SubsystemCache) return 0;
		return SubsystemCache->GetProgressionTowardsNextCoupon(EResourceSinkTrack::RST_Default);
	}

	UFUNCTION()
	TArray<double> GetPointsGraph()
	{
		if(!SubsystemCache) return TArray<double>();
		auto Graph = SubsystemCache->GetGlobalPointHistory(EResourceSinkTrack::RST_Default);
		TArray<double> Out;

		for (int32 Element : Graph)
			Out.Add(Element);

		return Out;
	}

	UFUNCTION()
	int GetPPM()
	{
		if(!SubsystemCache) return 0;
		return SubsystemCache->GetGlobalPointHistory(EResourceSinkTrack::RST_Default).Last();
	}



	UFUNCTION()
	FString GetTotalPoints_Exploration()
	{
		if(!SubsystemCache) return "";
		return FString::FromInt(SubsystemCache->GetNumTotalPoints(EResourceSinkTrack::RST_Exploration));
	}

	UFUNCTION()
	int GetPointsTillNextCoupon_Exploration()
	{
		if(!SubsystemCache) return 0;
		return SubsystemCache->GetNumPointsToNextCoupon(EResourceSinkTrack::RST_Exploration);
	}

	UFUNCTION()
	double GetProgress_Exploration()
	{
		if(!SubsystemCache) return 0;
		return SubsystemCache->GetProgressionTowardsNextCoupon(EResourceSinkTrack::RST_Exploration);
	}

	UFUNCTION()
	TArray<double> GetPointsGraph_Exploration()
	{
		if(!SubsystemCache) return TArray<double>();
		
		auto Graph = SubsystemCache->GetGlobalPointHistory(EResourceSinkTrack::RST_Exploration);
		TArray<double> Out;

		for (int32 Element : Graph)
			Out.Add(Element);

		return Out;
	}

	UFUNCTION()
	int GetPPM_Exploration()
	{
		if(!SubsystemCache) return 0;
		return SubsystemCache->GetGlobalPointHistory(EResourceSinkTrack::RST_Exploration).Last();
	}

	UPROPERTY()
	AFGResourceSinkSubsystem* SubsystemCache;
};
