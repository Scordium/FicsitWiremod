// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "CCArrayValueBase.h"
#include "CircuitryColorFilterRule.h"
#include "JsonUtilities.h"
#include "CCColorValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCColorValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCColorValue() : Super(Color){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCColorValue, Value)
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetColor)
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetColor(Pointer) == Value;
	}

	virtual FString ToString() override { return Value.ToString(); }

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		return MakeShareable(new FJsonValueString(UFGBlueprintFunctionLibrary::LinearColorToHex(Value)));
	}

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		return Value.InitFromString(Wrapper.Value);
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		return FDynamicValueStringWrapper(ConnectionType, ToString());
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FLinearColor Value;
};


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCColorArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCColorArrayValue() : Super(ArrayOfColor){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCColorArrayValue, Value)
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetColorArray)
	}

	virtual void AddElement(const FConnectionData& Element) override
	{
		auto NewElement = Element.GetColor();
		if (Filter.CheckFilterMatch(NewElement, true)) Value.Add(NewElement);
	}
	
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		UCCColorValue* ValueOut;
		if(auto OuterCast = Cast<UCCColorValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCColorValue>(this->GetWorld()->PersistentLevel);
		
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		auto NewElement = Element.GetColor();
		if (Filter.CheckFilterMatch(NewElement, true)) Value.Insert(NewElement, Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetColor();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetColor()); }
	
	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetColorArray(Pointer) == Value;
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		TArray<TSharedPtr<FJsonValue>> Array;

		for (const auto& ArrayValue : Value)
		{
			Array.Add(MakeShareable(new FJsonValueString(UFGBlueprintFunctionLibrary::LinearColorToHex(ArrayValue))));
		}

		return MakeShareable(new FJsonValueArray(Array));
	}

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		TArray<FString> StringValues;
		Wrapper.Value.ParseIntoArray(StringValues, *FString(ARRAY_SEPARATOR), false);
		Value.Empty();
		for(auto& StringValue : StringValues)
		{
			FLinearColor Color;
			Color.InitFromString(StringValue);
			Value.Add(Color);
		}

		return true;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		TArray<FString> Out;
		for(const auto& Val : Value) Out.Add(Val.ToString());
		
		const auto Output = FString::Join(Out, *FString(ARRAY_SEPARATOR));
		return FDynamicValueStringWrapper(ConnectionType, Output);
	}

	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(const auto& Val : Value)
			Out.Add(Val.ToString());

		return Out;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetColor();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetColor();
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
	TArray<FLinearColor> Value;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FCircuitryColorFilterRule Filter;
};
