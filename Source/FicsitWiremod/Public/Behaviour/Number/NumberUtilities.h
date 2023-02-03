// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
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

	UFUNCTION(BlueprintPure)
	static void QuadraticFormula(const FNewConnectionData& A, const FNewConnectionData& B, const FNewConnectionData& C, float& X1, float& X2)
	{
		X1 = 0;
		X2 = 0;
		
		float a = UWiremodReflection::GetFunctionNumberResult(A);
		float b = UWiremodReflection::GetFunctionNumberResult(B);
		float c = UWiremodReflection::GetFunctionNumberResult(C);

		if(a == 0 || b == 0 || c == 0) return;

		float D = b*b - 4*a*c;
		if(D == 0) return;

		X1 = (-b + sqrtf(D)) / (2*a);
		X2 = (-b - sqrtf(D)) / (2*a);
	}
	
};
