// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "ItemAmount.h"
#include "Behaviour/Gates/Arrays/Filter/Filters/CircuitryItemArrayFilter.h"
#include "CommonLib/FileUtilities.h"
#include "CCItemAmountValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCItemAmountValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCItemAmountValue() : Super(ItemAmount){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCItemAmountValue, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCItemAmountValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetItemAmount(REFLECTION_ARGS);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCItemAmountValue>(Other))
			return OtherSource->Value.Amount == Value.Amount && OtherSource->Value.ItemClass == Value.ItemClass;

		return false;
	}

	virtual FString ToString() override { return FString::FromInt(Value.Amount) + " of " + UFGItemDescriptor::GetItemName(Value.ItemClass).ToString();}

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		FString Class, Amount;
		Wrapper.Value.Split(";", &Class, &Amount);

		const auto ClassAsset = FSoftClassPath(Class).TryLoadClass<UFGItemDescriptor>();
		Value.ItemClass = ClassAsset;
		Value.Amount = FCString::Atoi(*Amount);
		return Value.ItemClass != nullptr;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		FString ValueString = FSoftClassPath(Value.ItemClass).ToString() + ";" + FString::FromInt(Value.Amount);
		return FDynamicValueStringWrapper(ConnectionType, ValueString);
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FItemAmount Value;
};


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCItemAmountArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCItemAmountArrayValue() : Super(ArrayOfItemAmount){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCItemAmountArrayValue, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCItemAmountArrayValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetItemAmountArray(REFLECTION_ARGS);
	}

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetItemAmount()); }
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		UCCItemAmountValue* ValueOut;
		if(auto OuterCast = Cast<UCCItemAmountValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCItemAmountValue>(this->GetWorld()->PersistentLevel);
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetItemAmount(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetItemAmount();
	}
	virtual bool Contains(const FConnectionData& Element) override
	{
		const auto B = Element.GetItemAmount();
		for(auto& A : Value)
			if(A.Amount == B.Amount && A.ItemClass == B.ItemClass)
				return true;
		
		return false;
	}
	
	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCItemAmountArrayValue>(Other))
		{
			if(OtherSource->Value.Num() != Value.Num()) return false;

			for(int i = 0; i < Value.Num(); i++)
			{
				auto A = OtherSource->Value[i];
				auto B = Value[i];

				bool Equal = A.Amount == B.Amount && A.ItemClass == B.ItemClass;
				if(!Equal) return false;
			}

			return true;
		}

		return false;
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		TArray<FString> StringValues;
		Wrapper.Value.ParseIntoArray(StringValues, *FString(ARRAY_SEPARATOR), false);
		Value.Empty();
		for(auto& StringValue : StringValues)
		{
			FString Class,Amount;
			StringValue.Split(";", &Class, &Amount);

			FItemAmount OutValue;
			const auto ClassAsset = FSoftClassPath(Class).TryLoadClass<UFGItemDescriptor>();
			OutValue.ItemClass = ClassAsset;
			OutValue.Amount = FCString::Atoi(*Amount);

			Value.Add(OutValue);
		}

		return true;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		TArray<FString> Out;
		for(const auto& Val : Value) Out.Add(FSoftClassPath(Val.ItemClass).ToString() + ";" + FString::FromInt(Val.Amount));
		
		const auto Output = FString::Join(Out, *FString(ARRAY_SEPARATOR));
		return FDynamicValueStringWrapper(ConnectionType, Output);
	}

	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(const auto& Val : Value)
			Out.Add(FString::FromInt(Val.Amount) + " of " + UFGItemDescriptor::GetItemName(Val.ItemClass).ToString());

		return Out;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetItemAmount();
		for(int i = 0; i < Value.Num(); i++)
		{
			auto A = Value[i];
			bool Equal = A.Amount == SearchValue.Amount && A.ItemClass == SearchValue.ItemClass;
			if(Equal) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetItemAmount();
		for(int i = 0; i < Value.Num(); i++)
		{
			auto A = Value[i];
			bool Equal = A.Amount == SearchValue.Amount && A.ItemClass == SearchValue.ItemClass;
			if(Equal) Out.Add(i);
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
		if(!Filter) Filter = NewObject<UCircuitryItemArrayFilter>(this);

		Filter->FilterType = ItemAmount;
		return Filter->FromJson(FilterData);
	}

	virtual void ApplyFilter() override
	{
		if(Filter) Value = Filter->FilterValues(Value);
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<FItemAmount> Value;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	UCircuitryItemArrayFilter* Filter = nullptr;
};
