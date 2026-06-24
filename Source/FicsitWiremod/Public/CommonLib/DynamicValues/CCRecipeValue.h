#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "Behaviour/Gates/Arrays/Filter/Rules/CircuitryRecipeFilterRule.h"
#include "FGRecipe.h"
#include "CCRecipeValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCRecipeValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCRecipeValue() : Super(Recipe){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCRecipeValue, Value)
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetRecipe)
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetRecipe(Pointer) == Value;
	}

	virtual FString ToString() override { return ::IsValid(Value) ? UFGRecipe::GetRecipeName(Value).ToString() : FString("N/A");}

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		return MakeShareable(new FJsonValueString(FSoftClassPath(Value).ToString()));
	}

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		const auto RecipeClass = FSoftClassPath(Wrapper.Value).TryLoadClass<UFGRecipe>();
		Value = RecipeClass;
		return Value != nullptr;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		return FDynamicValueStringWrapper(ConnectionType, FSoftClassPath(Value).ToString());
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TSubclassOf<UFGRecipe> Value;
};


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCRecipeArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()


public:

	UCCRecipeArrayValue() : Super(ArrayOfRecipe){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCRecipeArrayValue, Value)
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetRecipeArray)
	}

	virtual void AddElement(const FConnectionData& Element) override
	{
		auto NewElement = Element.GetRecipe();
		if (Filter.CheckFilterMatch(NewElement, true)) Value.Add(NewElement);
	}
	
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		UCCRecipeValue* ValueOut;
		if(auto OuterCast = Cast<UCCRecipeValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCRecipeValue>(this->GetWorld()->PersistentLevel);
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		auto NewElement = Element.GetRecipe();
		if (Filter.CheckFilterMatch(NewElement, true)) Value.Insert(NewElement, Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetRecipe();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetRecipe()); }
	
	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetRecipeArray(Pointer) == Value;
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		TArray<TSharedPtr<FJsonValue>> Array;

		for (const auto& ArrayValue : Value)
		{
			Array.Add(MakeShareable(new FJsonValueString(FSoftClassPath(ArrayValue).ToString())));
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
			Value.Add(FSoftClassPath(StringValue).TryLoadClass<UFGRecipe>());
		}

		return true;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		TArray<FString> Out;
		for(const auto& Val : Value) Out.Add(FSoftClassPath(Val).ToString());
		
		const auto Output = FString::Join(Out, *FString(ARRAY_SEPARATOR));
		return FDynamicValueStringWrapper(ConnectionType, Output);
	}
	
	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(const auto& Val : Value)
			Out.Add(IsValid(Val) ? UFGRecipe::GetRecipeName(Val).ToString() : "N/A");

		return Out;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetRecipe();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetRecipe();
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

	virtual void* GetFilterPtr() override { return &Filter; }
	virtual UScriptStruct* GetFilterStruct() override { return Filter.StaticStruct(); }

	virtual void OnValueUpdate() override
	{
		Filter.FilterValues(Value);
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<TSubclassOf<UFGRecipe>> Value;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FCircuitryRecipeFilterRule Filter;
};
