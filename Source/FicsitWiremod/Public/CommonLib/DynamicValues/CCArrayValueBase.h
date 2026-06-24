// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "Behaviour/Gates/Arrays/Filter/Rules/FilterRule.h"
#include "WiremodReflection.h"
#include "CommonLib/JsonUtilities.h"
#include "CCArrayValueBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class FICSITWIREMOD_API UCCArrayValueBase : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:

	UCCArrayValueBase(){}
	explicit UCCArrayValueBase(EConnectionType Type) : Super(Type){}
	
	UFUNCTION(BlueprintCallable)
	virtual void AddElement(const FConnectionData& Element)
	{
		DispatchNotImplemented("ADD_ELEMENT");
	}
	
	UFUNCTION(BlueprintCallable)
	virtual void Clear()
	{
		DispatchNotImplemented("CLEAR_ARRAY");
	}
	
	UFUNCTION(BlueprintCallable)
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer)
	{
		DispatchNotImplemented("GET_ELEMENT");
		return nullptr;
	}
	
	UFUNCTION(BlueprintCallable)
	virtual void InsertElement(const FConnectionData& Element, int Index)
	{
		DispatchNotImplemented("INSERT_ELEMENT");
	}
	
	UFUNCTION(BlueprintCallable)
	virtual int Length()
	{
		DispatchNotImplemented("ARRAY_LENGTH");
		return 0;
	}
	
	UFUNCTION(BlueprintCallable)
	virtual void RemoveElement(int Index)
	{
		DispatchNotImplemented("REMOVE_ELEMENT");
	}
	
	UFUNCTION(BlueprintCallable)
	virtual void Resize(int NewSize)
	{
		DispatchNotImplemented("RESIZE_ARRAY");
	}
	
	UFUNCTION(BlueprintCallable)
	virtual void SetElement(const FConnectionData& Element, int Index)
	{
		DispatchNotImplemented("SET_ELEMENT");
	}
	
	UFUNCTION(BlueprintCallable)
	virtual bool Contains(const FConnectionData& Element)
	{
		DispatchNotImplemented("ARRAY_CONTAINS");
		return false;
	}

	UFUNCTION(BlueprintCallable)
	virtual int FindFirst(const FConnectionData& Element)
	{
		DispatchNotImplemented("ARRAY_FIND");
		return -1;
	}

	UFUNCTION(BlueprintCallable)
	virtual TArray<double> FindAll(const FConnectionData& Element)
	{
		DispatchNotImplemented("ARRAY_FIND_ALL");
		return TArray<double>();
	}

	UFUNCTION(BlueprintCallable)
	virtual void AppendArray(const UCCArrayValueBase* Array)
	{
		DispatchNotImplemented("ARRAY_APPEND");
	}

	UFUNCTION(BlueprintCallable)
	virtual bool SetFilter(const FCircuitryArrayFilterData& FilterData)
	{
		if (FilterData.FilterType != UConnectionTypeFunctions::ArrayToBase(ConnectionType)) return false;

		const auto FilterPtr = GetFilterPtr();
		const auto FilterStruct = GetFilterStruct();
		if (!FilterPtr || !FilterStruct) return false;
		
		auto DeserializerResult = UJsonUtilities::DeserializeJson(FilterData.JsonDataString, FilterStruct, FilterPtr);
		((FCircuitryFilterRule*) FilterPtr)->RuleUsed = true;
		
		return DeserializerResult;
	}

	virtual void* GetFilterPtr(){ return nullptr; }
	virtual UScriptStruct* GetFilterStruct(){ return nullptr; }
};
