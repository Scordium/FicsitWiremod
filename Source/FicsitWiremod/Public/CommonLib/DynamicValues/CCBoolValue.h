// 

#pragma once

#include "CoreMinimal.h"
#include "CCArrayValueBase.h"
#include "CCDynamicValueBase.h"
#include "CircuitryBoolFilterRule.h"
#include "JsonUtilities.h"
#include "CommonLib/ReflectionUtilities.h"
#include "CCBoolValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCBoolValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:

	UCCBoolValue() : Super(Boolean){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCBoolValue, Value)
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override { DYNAMIC_FROMPOINTER(GetBool) }

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetBool(Pointer) == Value;
	}

	virtual FString ToString() override { return Value ? "true" : "false"; }

	virtual TSharedPtr<FJsonValue> ToJson() override { return MakeShareable(new FJsonValueBoolean(Value)); }

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		Value = Wrapper.Value.Equals("true", ESearchCase::IgnoreCase);
		return true;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		return FDynamicValueStringWrapper(ConnectionType, ToString());
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	bool Value;
};


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCBoolArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:

	UCCBoolArrayValue() : Super(ArrayOfBoolean){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCBoolArrayValue, Value)
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override { DYNAMIC_FROMPOINTER(GetBoolArray) }
	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		TArray<TSharedPtr<FJsonValue>> Array;

		for (const auto& ArrayValue : Value)
		{
			Array.Add(MakeShareable(new FJsonValueBoolean(ArrayValue)));
		}

		return MakeShareable(new FJsonValueArray(Array));
	}

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		TArray<FString> StringValues;
		Wrapper.Value.ParseIntoArray(StringValues, *FString(ARRAY_SEPARATOR), false);
		Value.Empty();
		for(auto& StringValue : StringValues) Value.Add(StringValue == "true");

		return true;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		TArray<FString> Out;
		for(const auto Val : Value) Out.Add(Val ? "true" : "false");
		
		const auto Output = FString::Join(Out, *FString(ARRAY_SEPARATOR));
		return FDynamicValueStringWrapper(ConnectionType, Output);
	}

	virtual void AddElement(const FConnectionData& Element) override
	{
		auto NewElement = Element.GetBool();
		if (Filter.CheckFilterMatch(NewElement, true)) Value.Add(NewElement);
	}
	
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;

		UCCBoolValue* ValueOut;
		if(auto OuterCast = Cast<UCCBoolValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCBoolValue>(this->GetWorld()->PersistentLevel);
		
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		auto NewElement = Element.GetBool();
		if (Filter.CheckFilterMatch(NewElement, true)) Value.Insert(NewElement, Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetBool();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetBool()); }
	
	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(const auto& Val : Value)
			Out.Add(Val ? "true" : "false");

		return Out;
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetBoolArray(Pointer) == Value;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetBool();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetBool();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) Out.Add(i);
		}

		return Out;
	}

	virtual void AppendArray(const UCCArrayValueBase* Array) override
	{
		if(auto ThisArray = Cast<ThisClass>(Array))
		{
			Value.Append(ThisArray->Value);
		}
	}

	virtual bool SetFilter(const FCircuitryArrayFilterData& FilterData) override
	{
		if (FilterData.FilterType != ConnectionType) return false;
		
		return UJsonUtilities::DeserializeJson(FilterData.JsonDataString, Filter.StaticStruct(), &Filter);
	}

	virtual void OnValueUpdate() override
	{
		Filter.FilterValues(Value);
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<bool> Value;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FCircuitryBoolFilterRule Filter;
};
