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
	static void SetElementText(AFGBuildable* sign, FString text, int32 elementIndex)
	{
		struct{ FString arg1; int32 elementIndex; } params{ text,elementIndex};

		auto data = FNewConnectionData(sign, "netFunc_Element_SetText");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementImageSize(AFGBuildable* sign, FVector2D size, int32 elementIndex)
	{
		struct{ FVector2D arg1; int32 elementIndex; } params{ size,elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetImageSize");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementOverwriteImageSize(AFGBuildable* sign, FVector2D size, int32 elementIndex)
	{
		struct{ FVector2D arg1; int32 elementIndex;} params{ size,elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetOverwriteImageSize");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementPannerType(AFGBuildable* sign, bool IsStep, int32 elementIndex)
	{
		struct{ bool arg1; int32 elementIndex; } params{ IsStep,elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetPannerType");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementStepFrequenz(AFGBuildable* sign, float frequenz, int32 elementIndex)
	{
		struct{ float arg1; int32 elementIndex; } params{ frequenz,elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetStepFrequenz");
		UWiremodReflection::ProcessFunction(data, &params);
		
	}

	UFUNCTION(BlueprintCallable)
	static void SetMaterialAnimation(AFGBuildable* sign, FLinearColor animationData)
	{
		struct{ FLinearColor arg1; } params{ animationData};
		auto data = FNewConnectionData(sign, "netFunc_Material_SetAnimation");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementColor(AFGBuildable* sign, FLinearColor color, int32 elementIndex)
	{
		struct{ FLinearColor arg1; int32 elementIndex; } params{ color,elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetColor");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementOpacity(AFGBuildable* sign, float opacity, int32 elementIndex)
	{
		struct{ float arg1; int32 elementIndex; } params{opacity, elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetOpacity");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementRotation(AFGBuildable* sign, float rotation, int32 elementIndex)
	{
		struct{ float arg1; int32 elementIndex; } params{ rotation,elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetRotation");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementTexture(AFGBuildable* sign, UTexture* texture, int32 elementIndex)
	{
		struct{ UTexture* arg1; int32 elementIndex; } params{ texture,elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetTexture");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementTextSize(AFGBuildable* sign, int size, int32 elementIndex)
	{
		struct{ int arg1; int32 elementIndex; } params{ size,elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetTextSize");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementIsBold(AFGBuildable* sign, bool isBold, int32 elementIndex)
	{
		struct{ bool arg1; int32 elementIndex; } params{isBold, elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetIsBold");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementIsUppercase(AFGBuildable* sign, bool isUppercase, int32 elementIndex)
	{
		struct{ bool arg1; int32 elementIndex; } params{ isUppercase,elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetUppercase");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementJustify(AFGBuildable* sign, int justify, int32 elementIndex)
	{
		struct{ int arg1; int32 elementIndex; } params{ justify,elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetJustify");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementZIndex(AFGBuildable* sign, int zIndex, int32 elementIndex)
	{
		struct{ int arg1; int32 elementIndex; } params{ zIndex,elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetZIndex");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetElementPosition(AFGBuildable* sign, FVector2D position, int32 elementIndex)
	{
		struct{ FVector2D arg1; int32 elementIndex; } params{ position,elementIndex};
		auto data = FNewConnectionData(sign, "netFunc_Element_SetPosition");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	UFUNCTION(BlueprintCallable)
	static void SetBackgroundColor(AFGBuildable* sign, FLinearColor color)
	{
		struct{ FLinearColor arg1; } params{ color};
		auto data = FNewConnectionData(sign, "netFunc_SetSignBackgroundColor");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	UFUNCTION(BlueprintCallable)
	static void SetEmissionLevel(AFGBuildable* sign, float emissive)
	{
		struct{ float arg1; } params{ emissive};
		auto data = FNewConnectionData(sign, "netFunc_SetSignEmmisive");
		UWiremodReflection::ProcessFunction(data, &params);
	}

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
