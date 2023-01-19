// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "ValueConverters.generated.h"

#define WM UWiremodReflection
#define STR UKismetStringLibrary
/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UValueConverters : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static FString BoolToString(const FNewConnectionData& In) { return WM::GetFunctionBoolResult(In) ? "true" : "false"; }

	UFUNCTION(BlueprintCallable)
	static FString NumberToString(const FNewConnectionData& In, const FNewConnectionData& Truncate, const FNewConnectionData& MinFractions, const FNewConnectionData& MaxFractions)
	{
		float value = WM::GetFunctionNumberResult(In);
		if(WM::IsInteger(In))
			return STR::Conv_IntToString(value);
		
		if(!WM::GetFunctionBoolResult(Truncate))
			return STR::Conv_FloatToString(value); 

		int minFractions = WM::GetFunctionNumberResult(MinFractions);
		int maxFractions = WM::GetFunctionNumberResult(MaxFractions);

		return UKismetTextLibrary::Conv_FloatToText(value, HalfToZero, false, true, 1, 324, minFractions, maxFractions).ToString();
		
	}

	UFUNCTION(BlueprintCallable)
	static FString VectorToString(const FNewConnectionData& In){ return STR::Conv_VectorToString(WM::GetFunctionVectorResult(In));}

	UFUNCTION(BlueprintCallable)
	static FString ColorToString(const FNewConnectionData& In){ return STR::Conv_ColorToString(WM::GetFunctionColorResult(In)); }

	UFUNCTION(BlueprintCallable)
	static FString RecipeToString(const FNewConnectionData& In){ return UFGRecipe::GetRecipeName(WM::GetFunctionRecipeResult(In)).ToString(); }

	UFUNCTION(BlueprintCallable)
	static FString NumberToTimeString(const FNewConnectionData& In){ return UFGBlueprintFunctionLibrary::SecondsToTimeString(WM::GetFunctionNumberResult(In)); }

	UFUNCTION(BlueprintCallable)
	static void BoolArrToStringArr(const FNewConnectionData& In, TArray<FString>& ReturnValue)
	{
		auto array = UWiremodReflection::GetBoolArray(In);
		for (bool element : array) { ReturnValue.Add(element ? "true" : "false"); }
	}

	UFUNCTION(BlueprintCallable)
	static void NumArrToStringArr(const FNewConnectionData& In, const FNewConnectionData& Truncate, const FNewConnectionData& MinFractions, const FNewConnectionData& MaxFractions, TArray<FString>& ReturnValue)
	{
		auto array = UWiremodReflection::GetNumberArray(In);
		bool truncate = UWiremodReflection::GetFunctionBoolResult(Truncate);
		int minFractions = UWiremodReflection::GetFunctionNumberResult(MinFractions);
		int maxFractions = UWiremodReflection::GetFunctionNumberResult(MaxFractions);

		
		for (float element : array)
		{
			if(truncate)
			{
				ReturnValue.Add
				(
					UKismetTextLibrary::Conv_FloatToText
					(
						element,
						HalfToZero,
						false,
						true,
						1,
						324,
						minFractions,
						maxFractions
					).ToString()
				);
				continue;
			}
			ReturnValue.Add(UKismetStringLibrary::Conv_FloatToString(element));
		}
	}
};
