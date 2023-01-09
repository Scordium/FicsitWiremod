// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "Kismet/KismetStringLibrary.h"
#include "ToString.generated.h"
#define WM UWiremodReflection
#define STR UKismetStringLibrary

//Making cursed even curseder
#define Connection const FNewConnectionData& In

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UToString : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static FString BoolToString(Connection) { return WM::GetFunctionBoolResult(In) ? "true" : "false"; }

	UFUNCTION(BlueprintCallable)
	static FString NumberToString(Connection)
	{
		if(WM::IsInteger(In))
			return STR::Conv_IntToString(WM::GetFunctionNumberResult(In));
		return STR::Conv_FloatToString(WM::GetFunctionNumberResult(In));
	}

	UFUNCTION(BlueprintCallable)
	static FString VectorToString(Connection){ return STR::Conv_VectorToString(WM::GetFunctionVectorResult(In));}

	UFUNCTION(BlueprintCallable)
	static FString ColorToString(Connection){ return STR::Conv_ColorToString(WM::GetFunctionColorResult(In)); }

	UFUNCTION(BlueprintCallable)
	static FString RecipeToString(Connection){ return UFGRecipe::GetRecipeName(WM::GetFunctionRecipeResult(In)).ToString(); }

	UFUNCTION(BlueprintCallable)
	static FString NumberToTimeString(Connection){ return UFGBlueprintFunctionLibrary::SecondsToTimeString(WM::GetFunctionNumberResult(In)); }
};
