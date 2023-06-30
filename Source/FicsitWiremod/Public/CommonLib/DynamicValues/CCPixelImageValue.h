#pragma once
#include "CCArrayValueBase.h"
#include "CCDynamicValueBase.h"
#include "Behaviour/Displays/PixelArtManager.h"

#include "CCPixelImageValue.generated.h"


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCPixelImageValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCPixelImageValue() : Super(PixelImage){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCPixelImageValue, Value)
	}

	virtual void SetValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCPixelImageValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetUnmanaged<FPixelScreenData>(REFLECTION_ARGS);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCPixelImageValue>(Other))
			return Value == OtherSource->Value;

		return false;
	}

	virtual FString ToString() override { return FString::FromInt(Value.Width) + "x" + FString::FromInt(Value.Height);}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FPixelScreenData Value;
};


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCPixelImageArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCPixelImageArrayValue() : Super(ArrayOfPixelImage){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCPixelImageArrayValue, Value)
	}

	virtual void SetValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCPixelImageArrayValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetUnmanaged<TArray<FPixelScreenData>>(REFLECTION_ARGS);
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(auto Val : Value)
			Out.Add(FString::FromInt(Val.Width) + "x" + FString::FromInt(Val.Height));

		return Out;
	}

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetPixelImage()); }
	virtual UCCDynamicValueBase* GetElement(int Index) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		auto ValueOut = NewObject<UCCPixelImageValue>(this->GetWorld()->PersistentLevel);
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetPixelImage(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetPixelImage();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetPixelImage()); }

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCPixelImageArrayValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<FPixelScreenData> Value;
};
