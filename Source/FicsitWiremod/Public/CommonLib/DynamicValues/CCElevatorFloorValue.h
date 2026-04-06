//

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "FGElevatorTypes.h"
#include "CCElevatorFloorValue.generated.h"

/*
 * 
 */
UCLASS()
class UCCElevatorFloorValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCElevatorFloorValue() : Super(ElevatorFloor){}

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, Value);
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetElevatorFloor)
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetElevatorFloor(Pointer) == Value;
	}

	virtual FString ToString() override
	{
		return Value.FloorName + " (" + FString::FromInt(Value.Height) + "m)";
	}

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		const TSharedRef<FJsonObject> Object = MakeShareable(new FJsonObject());

		Object->SetStringField("name", Value.FloorName);
		Object->SetNumberField("iconId", Value.IconId);
		Object->SetStringField("iconColor", UFGBlueprintFunctionLibrary::LinearColorToHex(Value.IconColor));
		Object->SetNumberField("height", Value.Height);

		return MakeShareable(new FJsonValueObject(Object));
	}

	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FElevatorFloorStopInfo Value;
	
};

UCLASS()
class UCCElevatorFloorArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCElevatorFloorArrayValue() : Super(ArrayOfElevatorFloor){}

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, Value);
	}

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER(GetElevatorFloorArray)
	}

	virtual void AddElement(const FConnectionData& Element) override
	{
		Value.Add(Element.GetElevatorFloor());
	}

	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;

		UCCElevatorFloorValue* ValueOut;
		if(auto OuterCast = Cast<UCCElevatorFloorValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCElevatorFloorValue>(this->GetWorld()->PersistentLevel);
		
		ValueOut->Value = Value[Index];
		return ValueOut;
	}

	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if (!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetElevatorFloor(), Index);
	}
	virtual void Clear() override { Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if (Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if (!Value.IsValidIndex(Index)) return;

		Value[Index] = Element.GetElevatorFloor();
	}

	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetElevatorFloor()); }
	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString("*")); }

	virtual TSharedPtr<FJsonValue> ToJson() override
	{
		TArray<TSharedPtr<FJsonValue>> Array;

		for (const auto& ArrayValue : Value)
		{
			const TSharedRef<FJsonObject> Object = MakeShareable(new FJsonObject());

			Object->SetStringField("name", ArrayValue.FloorName);
			Object->SetNumberField("iconId", ArrayValue.IconId);
			Object->SetStringField("iconColor", UFGBlueprintFunctionLibrary::LinearColorToHex(ArrayValue.IconColor));
			Object->SetNumberField("height", ArrayValue.Height);

			Array.Add(MakeShareable(new FJsonValueObject(Object)));
		}

		return MakeShareable(new FJsonValueArray(Array));
	}

	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(const auto& Val : Value)
			Out.Add(Val.FloorName + " (" + FString::FromInt(Val.Height) + "m)");

		return Out;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetElevatorFloor();
		for(int i = 0; i < Value.Num(); i++)
		{
			auto A = Value[i];
			if (A == SearchValue) return i;
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetElevatorFloor();
		for(int i = 0; i < Value.Num(); i++)
		{
			auto A = Value[i];
			if (A == SearchValue) Out.Add(i);
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
	TArray<FElevatorFloorStopInfo> Value;
};
