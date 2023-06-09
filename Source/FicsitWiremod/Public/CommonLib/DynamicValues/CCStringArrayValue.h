// 

#pragma once

#include "CoreMinimal.h"
#include "CCArrayValueBase.h"
#include "CCStringValue.h"
#include "CCStringArrayValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCStringArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCStringArrayValue() : Super(ArrayOfString){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCStringArrayValue, Value)
	}

	virtual void SetValue(const FValueReflectionSource& Source) override
	{
		if(!Source.Object) return;
		if(Source.Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCStringArrayValue>(IDynamicValuePasser::Execute_GetValue(Source.Object, Source.SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Source.SetByAddress(Value);
	}

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetString()); }
	virtual UCCDynamicValueBase* GetElement(int Index) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		auto ValueOut = NewObject<UCCStringValue>(this->GetWorld()->PersistentLevel);
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetString(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetString();
	}
	
	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCStringArrayValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(",")); }
	virtual TArray<FString> ToStringArray() override { return Value; }
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<FString> Value;
};
