// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "CommonLib/CustomStruct.h"
#include "CCCustomStructValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCCustomStructValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCCustomStructValue() : Super(CustomStruct){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCCustomStructValue, Value)
	}
	
	virtual void SetValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCCustomStructValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetUnmanaged<FCustomStruct>(REFLECTION_ARGS);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCCustomStructValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override { return Value.Name + "[" + FString::FromInt(Value.Values.Num()) + " values]"; }
	
	UPROPERTY(Replicated, SaveGame)
	FCustomStruct Value;
};


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCCustomStructArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCCustomStructArrayValue() : Super(ArrayOfCustomStruct){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCCustomStructArrayValue, Value)
	}

	virtual void SetValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCCustomStructArrayValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetUnmanaged<TArray<FCustomStruct>>(REFLECTION_ARGS);
	}

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetCustomStruct()); }
	virtual UCCDynamicValueBase* GetElement(int Index) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		auto ValueOut = NewObject<UCCCustomStructValue>(this->GetWorld()->PersistentLevel);
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetCustomStruct(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetCustomStruct();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetCustomStruct()); }
	
	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCCustomStructArrayValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(auto Val : Value)
			Out.Add(Val.Name + "[" + FString::FromInt(Val.Values.Num()) + " values]");

		return Out;
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<FCustomStruct> Value;
};