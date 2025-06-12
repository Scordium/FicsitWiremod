// 

#pragma once

#include "CoreMinimal.h"
#include "CCArrayValueBase.h"
#include "FGInventoryComponent.h"
#include "CommonLib/ReflectionUtilities.h"
#include "CCInventoryValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCInventoryValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCInventoryValue() : Super(Inventory){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCInventoryValue, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCInventoryValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetInventory(REFLECTION_ARGS);
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(UObject* Object, FName SourceName, bool FromProperty) override
	{
		return UReflectionUtilities::GetInventory(Object, SourceName, FromProperty) == Value;
	}

	virtual FString ToString() override
	{
		if(!Value) return "Invalid Inv.";

		TArray<FInventoryStack> Stacks;
		Value->GetInventoryStacks(Stacks);
				
		return FString::FromInt(Stacks.Num()) + "/" + FString::FromInt(Value->GetSizeLinear()) + " slots occupied";
	}

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		return UJsonUtilities::InventoryToJson(Value);
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	UFGInventoryComponent* Value;
};


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCInventoryArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCInventoryArrayValue() : Super(ArrayOfInventory){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCInventoryArrayValue, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCInventoryArrayValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetInventoryArray(REFLECTION_ARGS);
	}

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetInventory()); }
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		UCCInventoryValue* ValueOut;
		if(auto OuterCast = Cast<UCCInventoryValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCInventoryValue>(this->GetWorld()->PersistentLevel);
		
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetInventory(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetInventory();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetInventory()); }
	
	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(UObject* Object, FName SourceName, bool FromProperty) override
	{
		return UReflectionUtilities::GetInventoryArray(Object, SourceName, FromProperty) == Value;
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		TArray<TSharedPtr<FJsonValue>> Array;
		for (UFGInventoryComponent* Inventory : Value)
		{
			Array.Add(UJsonUtilities::InventoryToJson(Inventory));
		}

		return MakeShareable(new FJsonValueArray(Array));
	}

	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(const auto& Val : Value)
		{
			if(!Val)
			{
				Out.Add("Invalid Inv.");
				continue;
			}

			TArray<FInventoryStack> Stacks;
			Val->GetInventoryStacks(Stacks);
				
			Out.Add(FString::FromInt(Stacks.Num()) + "/" + FString::FromInt(Val->GetSizeLinear()) + " slots occupied");
		}

		return Out;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetInventory();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetInventory();
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
	TArray<UFGInventoryComponent*> Value;
};
