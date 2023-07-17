// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "CCNumberValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCNumberValue : public UCCDynamicValueBase
{
	GENERATED_BODY()


public:

	UCCNumberValue() : Super(Number){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCNumberValue, Value)
	}

	virtual void SetValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCNumberValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetFloat(REFLECTION_ARGS, false);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCNumberValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override { return FString::SanitizeFloat(Value); }
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	double Value;
};



UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCNumberArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCNumberArrayValue() : Super(ArrayOfNumber){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCNumberArrayValue, Value)
	}

	virtual void SetValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCNumberArrayValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetFloatArray(REFLECTION_ARGS);
	}	

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetFloat()); }
	virtual UCCDynamicValueBase* GetElement(int Index) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		auto ValueOut = NewObject<UCCNumberValue>(this->GetWorld()->PersistentLevel);
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetFloat(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetFloat();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetFloat()); }
	
	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCNumberArrayValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }
	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(auto Val : Value)
			Out.Add(FString::SanitizeFloat(Val));

		return Out;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetFloat();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetFloat();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) Out.Add(i);
		}

		return Out;
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<double> Value;
};
