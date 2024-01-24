// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "FGInventoryComponent.h"
#include "CCStackValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCStackValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:

	UCCStackValue() : Super(Stack){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCStackValue, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCStackValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetStack(REFLECTION_ARGS);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCStackValue>(Other))
			return OtherSource->Value.Item.GetItemClass() == Value.Item.GetItemClass() && OtherSource->Value.NumItems == Value.NumItems;

		return false;
	}

	virtual FString ToString() override { return FString::FromInt(Value.NumItems) + " " + UFGItemDescriptor::GetItemName(Value.Item.GetItemClass()).ToString(); }

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		FString Class, Amount;
		Wrapper.Value.Split(";", &Class, &Amount);

		auto ClassAsset = Cast<UFGItemDescriptor>(UFileUtilities::GetAssetByPath(FTopLevelAssetPath(Class)).GetAsset());
		if(!ClassAsset) return false;
		Value.Item.SetItemClass(ClassAsset->GetClass());
		Value.NumItems = FCString::Atoi(*Amount);
		return true;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		FString ValueString = Value.Item.GetItemClass()->GetClassPathName().ToString() + ";" + FString::FromInt(Value.NumItems);
		return FDynamicValueStringWrapper(ConnectionType, ValueString);
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FInventoryStack Value;
};


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCStackArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:

	UCCStackArrayValue() : Super(ArrayOfStack){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCStackArrayValue, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCStackArrayValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetStackArray(REFLECTION_ARGS);
	}

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetStack()); }
	virtual UCCDynamicValueBase* GetElement(int Index) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		auto ValueOut = NewObject<UCCStackValue>(this->GetWorld()->PersistentLevel);
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetStack(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetStack();
	}
	virtual bool Contains(const FConnectionData& Element) override
	{
		const auto B = Element.GetStack();
		for(auto& A : Value)
			if(A.NumItems == B.NumItems && A.Item.GetItemClass() == B.Item.GetItemClass())
				return true;
		
		return false;
	}
	
	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCStackArrayValue>(Other))
		{
			if(OtherSource->Value.Num() != Value.Num()) return false;

			for(int i = 0; i < Value.Num(); i++)
			{
				auto A = OtherSource->Value[i];
				auto B = Value[i];

				bool Equal = A.NumItems == B.NumItems && A.Item.GetItemClass() == B.Item.GetItemClass();
				if(!Equal) return false;
			}

			return true;
		}

		return false;
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }
	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(const auto& Val : Value)
			Out.Add(FString::FromInt(Val.NumItems) + " " + UFGItemDescriptor::GetItemName(Val.Item.GetItemClass()).ToString());

		return Out;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetStack();
		for(int i = 0; i < Value.Num(); i++)
		{
			auto A = Value[i];
			bool Equal = A.NumItems == SearchValue.NumItems && A.Item.GetItemClass() == SearchValue.Item.GetItemClass();
			
			if(Equal) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetStack();
		for(int i = 0; i < Value.Num(); i++)
		{
			auto A = Value[i];
			bool Equal = A.NumItems == SearchValue.NumItems && A.Item.GetItemClass() == SearchValue.Item.GetItemClass();
			if(Equal) Out.Add(i);
		}

		return Out;
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<FInventoryStack> Value;
};
