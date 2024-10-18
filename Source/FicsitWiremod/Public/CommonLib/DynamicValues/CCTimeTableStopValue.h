// 

#pragma once

#include "CoreMinimal.h"
#include "CCArrayValueBase.h"
#include "CCDynamicValueBase.h"
#include "Behaviour/Gates/Breakers/BreakTimeTableStop.h"
#include "CommonLib/ReflectionUtilities.h"
#include "CCTimeTableStopValue.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCCTimeTableStopValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCTimeTableStopValue() : Super(TrainStop){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<ThisClass>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetTrainStop(REFLECTION_ARGS);
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value.Station == Value.Station;

		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(UObject* Object, FName SourceName, bool FromProperty) override
	{
		return UReflectionUtilities::GetTrainStop(Object, SourceName, FromProperty).Station == Value.Station;
	}

	virtual FString ToString() override { return Value.Station ? Value.Station->GetStationName().ToString() : "N/A"; }
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FTimeTableStopData Value;
};


UCLASS(Blueprintable,BlueprintType)
class UCCTimeTableStopArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCTimeTableStopArrayValue() : Super(ArrayOfTrainStop){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<ThisClass>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetTrainStopArray(REFLECTION_ARGS);
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
		{
			if(Value.Num() != OtherSource->Value.Num()) return false;

			for(int i = 0; i < Value.Num(); i++)
			{
				if(Value[i].Station != OtherSource->Value[i].Station) return false;
			}

			return true;
		}

		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(UObject* Object, FName SourceName, bool FromProperty) override
	{
		auto OtherSource = UReflectionUtilities::GetTrainStopArray(Object, SourceName, FromProperty);

		if(Value.Num() != OtherSource.Num()) return false;
        
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i].Station != OtherSource[i].Station) return false;
		}
        
		return true;
	}

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetTimeTableStop()); }
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		UCCTimeTableStopValue* ValueOut;
		if(auto OuterCast = Cast<UCCTimeTableStopValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCTimeTableStopValue>(this->GetWorld()->PersistentLevel);
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetTimeTableStop(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetTimeTableStop();
	}
	virtual bool Contains(const FConnectionData& Element) override
	{
		auto TimeTableStop = Element.GetTimeTableStop();
		for(auto& Val : Value)
		{
			if(Val.Station == TimeTableStop.Station) return true;
		}

		return false;
	}

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(auto& Val : Value) Out.Add(Val.Station ? Val.Station->GetStationName().ToString() : "NULL");

		return Out;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetTimeTableStop();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i].Station == SearchValue.Station) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetTimeTableStop();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i].Station == SearchValue.Station) Out.Add(i);
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
	TArray<FTimeTableStopData> Value;
	
};