// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Filters/CircuitryArrayFilterBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Rules/CircuitryBoolFilterRule.h"
#include "Rules/CircuitryColorFilterRule.h"
#include "Rules/CircuitryItemFilterRule.h"
#include "Rules/CircuitryNumberFilterRule.h"
#include "Rules/CircuitryStringFilterRule.h"
#include "Rules/CircuitryVectorFilterRule.h"
#include "ArrayFilterFuncs.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UArrayFilterFuncs : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void WriteString(FString* Out, const UScriptStruct* Struct, const void* StructPtr)
	{
		auto Writer = TJsonWriterFactory<>::Create(Out, 0);
		TSharedRef<FJsonObject> Object = MakeShared<FJsonObject>();
		FJsonObjectConverter::UStructToJsonObject(Struct, StructPtr, Object);
		
		FJsonSerializer::Serialize(Object, Writer);
	}

	UFUNCTION(BlueprintCallable)
	static bool BoolRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryBoolFilterRule& Out)
	{
		if(Data.FilterType != Boolean) return false;
		
		TSharedPtr<FJsonObject> Object;
		if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Data.JsonDataString), Object)) return false;

		return FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData BoolRuleToData(const FCircuitryBoolFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = Boolean;
		WriteString(&Out.JsonDataString, In.StaticStruct(), &In);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool NumberRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryNumberFilterRule& Out)
	{
		if(Data.FilterType != Number) return false;
		
		TSharedPtr<FJsonObject> Object;
		if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Data.JsonDataString), Object)) return false;

		return FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData NumberRuleToData(const FCircuitryNumberFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = Number;
		WriteString(&Out.JsonDataString, In.StaticStruct(), &In);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool StringRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryStringFilterRule& Out)
	{
		if(Data.FilterType != String) return false;
		
		TSharedPtr<FJsonObject> Object;
		if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Data.JsonDataString), Object)) return false;

		return FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData StringRuleToData(const FCircuitryStringFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = String;
		WriteString(&Out.JsonDataString, In.StaticStruct(), &In);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool VectorRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryVectorFilterRule& Out)
	{
		if(Data.FilterType != Vector) return false;
		
		TSharedPtr<FJsonObject> Object;
		if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Data.JsonDataString), Object)) return false;

		return FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData VectorRuleToData(const FCircuitryVectorFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = Vector;
		WriteString(&Out.JsonDataString, In.StaticStruct(), &In);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool ColorRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryColorFilterRule& Out)
	{
		if(Data.FilterType != Color) return false;
		
		TSharedPtr<FJsonObject> Object;
		if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Data.JsonDataString), Object)) return false;

		return FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData ColorRuleToData(const FCircuitryColorFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = Color;
		WriteString(&Out.JsonDataString, In.StaticStruct(), &In);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool ItemRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryItemFilterRule& Out)
	{
		if(Data.FilterType != ItemAmount) return false;
		
		TSharedPtr<FJsonObject> Object;
		if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Data.JsonDataString), Object)) return false;

		return FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData ItemRuleToData(const FCircuitryItemFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = ItemAmount;
		WriteString(&Out.JsonDataString, In.StaticStruct(), &In);
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static bool ItemDescriptorRuleFromData(const FCircuitryArrayFilterData& Data, FCircuitryItemDescriptorFilterRule& Out)
	{
		if(Data.FilterType != ItemDescriptor) return false;
		
		TSharedPtr<FJsonObject> Object;
		if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Data.JsonDataString), Object)) return false;

		return FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), &Out);
	}

	UFUNCTION(BlueprintCallable)
	static FCircuitryArrayFilterData ItemDescriptorRuleToData(const FCircuitryItemDescriptorFilterRule& In)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = ItemDescriptor;
		WriteString(&Out.JsonDataString, In.StaticStruct(), &In);
		
		return Out;
	}

	UFUNCTION(BlueprintPure)
	static bool DataEqual(const FCircuitryArrayFilterData& Data1, const FCircuitryArrayFilterData& Data2) { return Data1 == Data2; }
};
