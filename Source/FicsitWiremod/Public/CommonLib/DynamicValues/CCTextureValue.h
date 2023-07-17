// 

#pragma once

#include "CoreMinimal.h"
#include "CCArrayValueBase.h"
#include "CCDynamicValueBase.h"
#include "CommonLib/ReflectionUtilities.h"
#include "CCTextureValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCTextureValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:

	UCCTextureValue() : Super(Texture){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCTextureValue, Value)
	}

	virtual void SetValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCTextureValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetTexture(REFLECTION_ARGS);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCTextureValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override { return "?"; }
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	UTexture* Value;
};


UCLASS(Blueprintable,BlueprintType)
class UCCTextureArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCTextureArrayValue() : Super(ArrayOfTexture){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCTextureArrayValue, Value)
	}

	virtual void SetValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCTextureArrayValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetTextureArray(REFLECTION_ARGS);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCTextureArrayValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetTexture()); }
	virtual UCCDynamicValueBase* GetElement(int Index) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		auto ValueOut = NewObject<UCCTextureValue>(this->GetWorld()->PersistentLevel);
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetTexture(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetTexture();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetTexture()); }

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(auto Val : Value) Out.Add("?");

		return Out;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetTexture();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetTexture();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) Out.Add(i);
		}

		return Out;
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<UTexture*> Value;
	
};
