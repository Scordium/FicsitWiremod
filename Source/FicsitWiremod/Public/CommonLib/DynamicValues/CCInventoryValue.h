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

	virtual void SetValue(UObject* Object, FName SourceName, bool FromProperty) override
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

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCInventoryValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override
	{
		if(!Value) return "Invalid Inv.";

		TArray<FInventoryStack> Stacks;
		Value->GetInventoryStacks(Stacks);
				
		return FString::FromInt(Stacks.Num()) + "/" + FString::FromInt(Value->GetSizeLinear()) + " slots occupied";
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

	virtual void SetValue(UObject* Object, FName SourceName, bool FromProperty) override
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
	virtual UCCDynamicValueBase* GetElement(int Index) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		auto ValueOut = NewObject<UCCInventoryValue>(this->GetWorld()->PersistentLevel);
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
	virtual void RemoveElement(int Index) override { Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetInventory();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetInventory()); }
	
	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCInventoryArrayValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(auto Val : Value)
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
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<UFGInventoryComponent*> Value;
};
