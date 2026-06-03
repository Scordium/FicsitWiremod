// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "Behaviour/Gates/Arrays/Filter/Rules/CircuitryVectorFilterRule.h"
#include "CCVectorValue.generated.h"
/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCVectorValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:

	UCCVectorValue() : Super(Vector){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCVectorValue, Value)
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetVector)
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetVector(Pointer) == Value;
	}

	virtual FString ToString() override { return Value.ToCompactString(); }

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		const TSharedRef<FJsonObject> Object = MakeShareable(new FJsonObject());

		Object->SetNumberField("X", Value.X);
		Object->SetNumberField("Y", Value.Y);
		Object->SetNumberField("Z", Value.Z);

		return MakeShareable(new FJsonValueObject(Object));
	}

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		return Value.InitFromString(Wrapper.Value);
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		return FDynamicValueStringWrapper(ConnectionType, Value.ToString());
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FVector Value;
};


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCVectorArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:

	UCCVectorArrayValue() : Super(ArrayOfVector){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCVectorArrayValue, Value)
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetVectorArray)
	}

	virtual void AddElement(const FConnectionData& Element) override
	{
		auto NewElement = Element.GetVector();
		if (Filter.CheckFilterMatch(NewElement, true)) Value.Add(NewElement);
	}
	
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		UCCVectorValue* ValueOut;
		if(auto OuterCast = Cast<UCCVectorValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCVectorValue>(this->GetWorld()->PersistentLevel);
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		auto NewElement = Element.GetVector();
		if (Filter.CheckFilterMatch(NewElement, true)) Value.Insert(NewElement, Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetVector();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetVector()); }
	
	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetVectorArray(Pointer) == Value;
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		TArray<TSharedPtr<FJsonValue>> Array;

		for (const auto& ArrayValue : Value)
		{
			const TSharedRef<FJsonObject> Object = MakeShareable(new FJsonObject());

			Object->SetNumberField("X", ArrayValue.X);
			Object->SetNumberField("Y", ArrayValue.Y);
			Object->SetNumberField("Z", ArrayValue.Z);
			
			Array.Add(MakeShareable(new FJsonValueObject(Object)));
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
			FVector Vec;
			Vec.InitFromString(StringValue);
			Value.Add(Vec);
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
			Out.Add(Val.ToCompactString());

		return Out;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetVector();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetVector();
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
		if (FilterData.FilterType != UConnectionTypeFunctions::ArrayToBase(ConnectionType)) return false;
		
		return UJsonUtilities::DeserializeJson(FilterData.JsonDataString, Filter.StaticStruct(), &Filter);
	}

	virtual void OnValueUpdate() override
	{
		Filter.FilterValues(Value);
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<FVector> Value;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FCircuitryVectorFilterRule Filter;
};
