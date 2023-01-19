// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "UObject/Object.h"
#include "NumberUtilities.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UNumberUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	static float CalculateAverageFromData(const FNewConnectionData& data)
	{
		return CalculateAverage(UWiremodReflection::GetNumberArray(data));
	}

	UFUNCTION(BlueprintPure)
	static float CalculateAverageFromDataArray(TArray<FNewConnectionData> data)
	{
		if(data.Num() == 0) return 0;
		
		float a = 0;
		for (FNewConnectionData Element : data){ a += UWiremodReflection::GetFunctionNumberResult(Element); }
		
		return a / data.Num();
	}

	UFUNCTION(BlueprintPure)
	static float CalculateAverage(TArray<float> In)
	{
		if(In.Num() == 0) return 0;
		
		float a = 0;
		for (float Element : In){ a += Element; }
		
		return a / In.Num();
	}
	
};
