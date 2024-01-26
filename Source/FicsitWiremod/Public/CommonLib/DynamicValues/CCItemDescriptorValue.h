// 

#pragma once

#include "CoreMinimal.h"
#include "CCArrayValueBase.h"
#include "CCDynamicValueBase.h"
#include "CommonLib/FileUtilities.h"
#include "CommonLib/ReflectionUtilities.h"
#include "CCItemDescriptorValue.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCCItemDescriptorValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:

	UCCItemDescriptorValue() : Super(ItemDescriptor){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCItemDescriptorValue, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCItemDescriptorValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetItemDescriptor(REFLECTION_ARGS, Value);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCItemDescriptorValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override
	{
		auto CDO = Value.GetDefaultObject();
		if(!CDO) return "None";

		return CDO->mDisplayName.ToString();
	}

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		const auto Descriptor = FSoftClassPath(Wrapper.Value).TryLoadClass<UFGItemDescriptor>();
		Value = Descriptor;
		return Value != nullptr;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		return FDynamicValueStringWrapper(ConnectionType, FSoftClassPath(Value).ToString());
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TSubclassOf<UFGItemDescriptor> Value;
};


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCItemDescriptorArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:

	UCCItemDescriptorArrayValue() : Super(ArrayOfItemDescriptor){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCItemDescriptorArrayValue, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCItemDescriptorArrayValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetItemDescriptorArray(REFLECTION_ARGS);
	}
	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetItemDescriptor()); }
	virtual UCCDynamicValueBase* GetElement(int Index) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		auto ValueOut = NewObject<UCCItemDescriptorValue>(this->GetWorld()->PersistentLevel);
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetItemDescriptor(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetItemDescriptor();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetItemDescriptor()); }
	
	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(const auto& Val : Value)
		{
			auto CDO = Val.GetDefaultObject();
			Out.Add(CDO->mDisplayName.ToString());
		}

		return Out;
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCItemDescriptorArrayValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetItemDescriptor();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetItemDescriptor();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) Out.Add(i);
		}

		return Out;
	}

	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<TSubclassOf<UFGItemDescriptor>> Value;
};
