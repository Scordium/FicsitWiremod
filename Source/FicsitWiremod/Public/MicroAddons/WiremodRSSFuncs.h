// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "UObject/Object.h"
#include "WiremodRSSFuncs.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiremodRSSFuncs : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static int GetElementCount(AFGBuildable* sign)
	{
		struct{ int ReturnValue; } params{};
		auto data = FNewConnectionData(sign, "netFunc_GetNumOfElements");
		UWiremodReflection::ProcessFunction(data, &params);
		return params.ReturnValue;
	}

	UFUNCTION(BlueprintCallable)
	static int GetElementCountOfType(AFGBuildable* sign, int type)
	{
		struct{ int type; int ReturnValue; } params{type};
		auto data = FNewConnectionData(sign, "netFunc_GetNumOfElementsOfType");
		UWiremodReflection::ProcessFunction(data, &params);
		return params.ReturnValue;
	}

	UFUNCTION(BlueprintCallable)
	static int GetSignType(AFGBuildable* sign)
	{
		struct{ int ReturnValue; } params{};
		auto data = FNewConnectionData(sign, "netFunc_GetSignType");
		UWiremodReflection::ProcessFunction(data, &params);
		return params.ReturnValue;
	}
};
