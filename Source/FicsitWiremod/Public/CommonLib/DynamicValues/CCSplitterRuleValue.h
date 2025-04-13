// 

#pragma once

#include "CoreMinimal.h"
#include "CCArrayValueBase.h"
#include "CCDynamicValueBase.h"
#include "Buildables/FGBuildableSplitterSmart.h"
#include "CommonLib/ReflectionUtilities.h"
#include "CCSplitterRuleValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FICSITWIREMOD_API UCCSplitterRuleValue : public UCCDynamicValueBase
{
	GENERATED_BODY()
	
public:
	UCCSplitterRuleValue() : Super(SplitterRule){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCSplitterRuleValue, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCSplitterRuleValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetSplitterRule(REFLECTION_ARGS);
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value.ItemClass == Value.ItemClass && OtherSource->Value.OutputIndex == Value.OutputIndex;

		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(UObject* Object, FName SourceName, bool FromProperty) override
	{
		auto OtherSource = UReflectionUtilities::GetSplitterRule(Object, SourceName, FromProperty);
		return OtherSource.ItemClass == Value.ItemClass && OtherSource.OutputIndex == Value.OutputIndex;
	}

	virtual FString ToString() override
	{
		auto Name = FStringFormatArg(Value.ItemClass == nullptr ? "N/A" : UFGItemDescriptor::GetItemName(Value.ItemClass).ToString());
		auto Index = FStringFormatArg(Value.OutputIndex);
		auto Args = FStringFormatOrderedArguments(TArray{Name, Index});
		return FString::Format(*FString("{0} [{1}]"), Args);
	}

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		const TSharedRef<FJsonObject> Object = MakeShareable(new FJsonObject());

		Object->SetField("ItemClass", MakeShareable(new FJsonValueString(FSoftClassPath(Value.ItemClass).ToString())));
		Object->SetNumberField("OutputIndex", Value.OutputIndex);

		return MakeShareable(new FJsonValueObject(Object));
	}

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		FString Class, OutputIndex;
		Wrapper.Value.Split(";", &Class, &OutputIndex);

		const auto ItemClass = FSoftClassPath(Class).TryLoadClass<UFGItemDescriptor>();
		Value.ItemClass = ItemClass;
		Value.OutputIndex = FCString::Atoi(*OutputIndex);
		
		return Value.ItemClass != nullptr;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		FString ValueString = FSoftClassPath(Value.ItemClass).ToString() + ";" + FString::FromInt(Value.OutputIndex);
		return FDynamicValueStringWrapper(ConnectionType, ValueString);
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FSplitterSortRule Value;
};


UCLASS(Blueprintable, BlueprintType)
class FICSITWIREMOD_API UCCSplitterRuleArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:

	UCCSplitterRuleArrayValue() : Super(ArrayOfSplitterRule){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCSplitterRuleArrayValue, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCSplitterRuleArrayValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetSplitterRuleArray(REFLECTION_ARGS);
	}
	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		TArray<TSharedPtr<FJsonValue>> Array;

		for (const auto& ArrayValue : Value)
		{
			const TSharedRef<FJsonObject> Object = MakeShareable(new FJsonObject());

			Object->SetField("ItemClass", MakeShareable(new FJsonValueString(FSoftClassPath(ArrayValue.ItemClass).ToString())));
			Object->SetNumberField("OutputIndex", ArrayValue.OutputIndex);

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
			FString Class,OutputIndex;
			StringValue.Split(";", &Class, &OutputIndex);

			FSplitterSortRule OutValue;
			const auto ClassAsset = FSoftClassPath(Class).TryLoadClass<UFGItemDescriptor>();
			OutValue.ItemClass = ClassAsset;
			OutValue.OutputIndex = FCString::Atoi(*OutputIndex);

			Value.Add(OutValue);
		}

		return true;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		TArray<FString> Out;
		for(const auto& Val : Value) Out.Add(FSoftClassPath(Val.ItemClass).ToString() + ";" + FString::FromInt(Val.OutputIndex));
		
		const auto Output = FString::Join(Out, *FString(ARRAY_SEPARATOR));
		return FDynamicValueStringWrapper(ConnectionType, Output);
	}

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetSplitterRule()); }
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		UCCSplitterRuleValue* ValueOut;
		if(auto OuterCast = Cast<UCCSplitterRuleValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCSplitterRuleValue>(this->GetWorld()->PersistentLevel);
		
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetSplitterRule(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetSplitterRule();
	}
	virtual bool Contains(const FConnectionData& Element) override
	{
		const auto B = Element.GetSplitterRule();
		for(const auto& A : Value)
			if(A.ItemClass == B.ItemClass && A.OutputIndex == B.OutputIndex)
				return true;
		
		return false;
	}
	
	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(const auto& Val : Value)
		{
			auto Name = FStringFormatArg(Val.ItemClass == nullptr ? "N/A" : UFGItemDescriptor::GetItemName(Val.ItemClass).ToString());
			auto Index = FStringFormatArg(Val.OutputIndex);
			auto Args = FStringFormatOrderedArguments(TArray{Name, Index});
			Out.Add(FString::Format(*FString("{0} [{1}]"), Args));
		}

		return Out;
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
		{
			if(OtherSource->Value.Num() != Value.Num()) return false;

			for(int i = 0; i < Value.Num(); i++)
			{
				auto A = OtherSource->Value[i];
				auto B = Value[i];

				bool Equal = A.ItemClass == B.ItemClass && A.OutputIndex == B.OutputIndex;
				if(!Equal) return false;
			}

			return true;
		}

		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(UObject* Object, FName SourceName, bool FromProperty) override
	{
		auto OtherSource = UReflectionUtilities::GetSplitterRuleArray(Object, SourceName, FromProperty);

		if(OtherSource.Num() != Value.Num()) return false;
        
		for(int i = 0; i < Value.Num(); i++)
		{
			auto A = OtherSource[i];
			auto B = Value[i];
        
			bool Equal = A.ItemClass == B.ItemClass && A.OutputIndex == B.OutputIndex;
			if(!Equal) return false;
		}
        
		return true;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetSplitterRule();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i].ItemClass == SearchValue.ItemClass && Value[i].OutputIndex == SearchValue.OutputIndex) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetSplitterRule();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i].ItemClass == SearchValue.ItemClass && Value[i].OutputIndex == SearchValue.OutputIndex) Out.Add(i);
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

	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<FSplitterSortRule> Value;
};
