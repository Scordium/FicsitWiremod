// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "Buildables/FGBuildable.h"
#include "UObject/Object.h"
#include "WiremodLinearMotionFuncs.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiremodLinearMotionFuncs : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static void SetFloorHeight(AFGBuildable* Lift, int FloorIndex, float Height)
	{
		auto data = FNewConnectionData(Lift, "netFunc_SetFloorHeight");

		struct { int floorIndex; float height; } params {FloorIndex, Height};
		UWiremodReflection::ProcessFunction(data, params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetFloorName(AFGBuildable* lift, int FloorIndex, FString Name)
	{
		auto data = FNewConnectionData(lift, "netFunc_SetFloorName");
		struct { int floorIndex; FString name; } params {FloorIndex, Name};
		UWiremodReflection::ProcessFunction(data, params);
	}


	UFUNCTION(BlueprintCallable)
	static void GetFloorInfo(AFGBuildable* Lift, int FloorIndex, FString& Name, float& Height)
	{
		auto data = FNewConnectionData(Lift, "netFunc_GetFloorInfo");
		struct {int floorIndex; float out_Height; FString out_Name; } params{FloorIndex};

		auto result = UWiremodReflection::ProcessFunction(data, params);
		Name = result.out_Name;
		Height = result.out_Height;
	}
};
