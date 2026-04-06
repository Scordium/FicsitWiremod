// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "FGPowerCircuit.h"
#include "CCCircuitValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCCircuitValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCCircuitValue() : Super(PowerGrid){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCCircuitValue, Value)
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetCircuit)
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetCircuit(Pointer) == Value;
	}

	virtual FString ToString() override { return "?"; }
	
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	UFGPowerCircuit* Value;
};


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCCircuitArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCCircuitArrayValue() : Super(ArrayOfPowerGrid){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCCircuitArrayValue, Value)
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetCircuitArray)
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(const auto& Val : Value)
			Out.Add("?");

		return Out;
	}

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetCircuit()); }
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		UCCCircuitValue* ValueOut;
		if(auto OuterCast = Cast<UCCCircuitValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCCircuitValue>(this->GetWorld()->PersistentLevel);
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetCircuit(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetCircuit();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetCircuit()); }

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetCircuitArray(Pointer) == Value;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetCircuit();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetCircuit();
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
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<UFGPowerCircuit*> Value;
};
