// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonUtilities.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Rules/CircuitryBoolFilterRule.h"
#include "Rules/CircuitryColorFilterRule.h"
#include "Rules/CircuitryItemFilterRule.h"
#include "Rules/CircuitryNumberFilterRule.h"
#include "Rules/CircuitryStringFilterRule.h"
#include "Rules/CircuitryVectorFilterRule.h"
#include "Rules/CircuitryRecipeFilterRule.h"
#include "ArrayFilterFuncs.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UArrayFilterFuncs : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static bool BoolRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryBoolFilterRule& Out)
	{
		if(Data.FilterType != Boolean) return false;
		
		return UJsonUtilities::DeserializeJson(Data.JsonDataString, Out.StaticStruct(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData BoolRuleToData(const FCircuitryBoolFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = Boolean;
		UJsonUtilities::SerializeJson(In.StaticStruct(), &In, &Out.JsonDataString);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool NumberRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryNumberFilterRule& Out)
	{
		if(Data.FilterType != Number) return false;
		
		return UJsonUtilities::DeserializeJson(Data.JsonDataString, Out.StaticStruct(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData NumberRuleToData(const FCircuitryNumberFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = Number;
		UJsonUtilities::SerializeJson(In.StaticStruct(), &In, &Out.JsonDataString);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool StringRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryStringFilterRule& Out)
	{
		if(Data.FilterType != String) return false;
		
		return UJsonUtilities::DeserializeJson(Data.JsonDataString, Out.StaticStruct(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData StringRuleToData(const FCircuitryStringFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = String;
		UJsonUtilities::SerializeJson(In.StaticStruct(), &In, &Out.JsonDataString);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool VectorRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryVectorFilterRule& Out)
	{
		if(Data.FilterType != Vector) return false;
		
		return UJsonUtilities::DeserializeJson(Data.JsonDataString, Out.StaticStruct(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData VectorRuleToData(const FCircuitryVectorFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = Vector;
		UJsonUtilities::SerializeJson(In.StaticStruct(), &In, &Out.JsonDataString);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool ColorRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryColorFilterRule& Out)
	{
		if(Data.FilterType != Color) return false;
		
		return UJsonUtilities::DeserializeJson(Data.JsonDataString, Out.StaticStruct(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData ColorRuleToData(const FCircuitryColorFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = Color;
		UJsonUtilities::SerializeJson(In.StaticStruct(), &In, &Out.JsonDataString);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool ItemRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryItemFilterRule& Out)
	{
		if(Data.FilterType != ItemAmount) return false;
		
		return UJsonUtilities::DeserializeJson(Data.JsonDataString, Out.StaticStruct(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData ItemRuleToData(const FCircuitryItemFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = ItemAmount;
		UJsonUtilities::SerializeJson(In.StaticStruct(), &In, &Out.JsonDataString);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool ItemDescriptorRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryItemDescriptorFilterRule& Out)
	{
		if(Data.FilterType != ItemDescriptor) return false;
		
		return UJsonUtilities::DeserializeJson(Data.JsonDataString, Out.StaticStruct(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData ItemDescriptorRuleToData(const FCircuitryItemDescriptorFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = ItemDescriptor;
		UJsonUtilities::SerializeJson(In.StaticStruct(), &In, &Out.JsonDataString);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool RecipeRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryRecipeFilterRule& Out)
	{
		if (Data.FilterType != Recipe) return false;

		return UJsonUtilities::DeserializeJson(Data.JsonDataString, Out.StaticStruct(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData RecipeRuleToData(const FCircuitryRecipeFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = Recipe;
		UJsonUtilities::SerializeJson(In.StaticStruct(), &In, &Out.JsonDataString);
		
		return Out;
	}

	UFUNCTION(BlueprintPure)
	static bool DataEqual(const FCircuitryArrayFilterData& Data1, const FCircuitryArrayFilterData& Data2) { return Data1 == Data2; }
};
