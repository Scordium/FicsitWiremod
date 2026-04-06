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

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetPixelImage)
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetUnmanaged<FPixelScreenData>(Pointer) == Value;
	}

	virtual FString ToString() override { return FString::FromInt(Value.Width) + "x" + FString::FromInt(Value.Height);}

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		return MakeShareable(new FJsonValueString("Why."));
	}
	
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

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetPixelImageArray)
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(const auto& Val : Value)
			Out.Add(FString::FromInt(Val.Width) + "x" + FString::FromInt(Val.Height));

		return Out;
	}

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetPixelImage()); }
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		UCCPixelImageValue* ValueOut;
		if(auto OuterCast = Cast<UCCPixelImageValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCPixelImageValue>(this->GetWorld()->PersistentLevel);
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
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetPixelImage();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetPixelImage()); }

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetUnmanaged<TArray<FPixelScreenData>>(Pointer) == Value;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetPixelImage();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetPixelImage();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) Out.Add(i);
		}

		return Out;
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<FPixelScreenData> Value;
};
