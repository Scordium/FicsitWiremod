// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "CCArrayValueBase.h"
#include "JsonUtilities.h"
#include "CircuitryStringFilterRule.h"
#include "CommonLib/ReflectionUtilities.h"
#include "CCStringValue.generated.h"


/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCStringValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:

	UCCStringValue() : Super(String){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCStringValue, Value)
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetString)
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetString(Pointer) == Value;
	}

	virtual FString ToString() override { return Value; }

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		return MakeShareable(new FJsonValueString(Value));
	}
	
	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		Value = Wrapper.Value;
		return true;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override { return FDynamicValueStringWrapper(ConnectionType, Value); }
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FString Value;
};



UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCStringArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCStringArrayValue() : Super(ArrayOfString){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCStringArrayValue, Value)
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetStringArray)
	}

	virtual void AddElement(const FConnectionData& Element) override
	{
		auto NewElement = Element.GetString();
		if (Filter.CheckFilterMatch(NewElement, true)) Value.Add(NewElement);
	}
	
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		UCCStringValue* ValueOut;
		if(auto OuterCast = Cast<UCCStringValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCStringValue>(this->GetWorld()->PersistentLevel);
		
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		auto NewElement = Element.GetString();
		if (Filter.CheckFilterMatch(NewElement, true)) Value.Insert(NewElement, Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetString();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetString()); }
	
	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetStringArray(Pointer) == Value;
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		TArray<TSharedPtr<FJsonValue>> Array;

		for (const auto& ArrayValue : Value)
		{
			Array.Add(MakeShareable(new FJsonValueString(ArrayValue)));
		}

		return MakeShareable(new FJsonValueArray(Array));
	}

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		Value.Empty();
		Wrapper.Value.ParseIntoArray(Value, *FString(ARRAY_SEPARATOR));

		return true;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		const auto Output = FString::Join(Value, *FString(ARRAY_SEPARATOR));
		return FDynamicValueStringWrapper(ConnectionType, Output);
	}
	
	virtual TArray<FString> ToStringArray() override { return Value; }

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetString();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetString();
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
	TArray<FString> Value;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FCircuitryStringFilterRule Filter;
};
