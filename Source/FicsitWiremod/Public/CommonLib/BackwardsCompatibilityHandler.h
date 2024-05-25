// 

#pragma once

#include "CoreMinimal.h"
#include "DynamicValues/CCColorValue.h"
#include "DynamicValues/CCDynamicValueBase.h"
#include "BackwardsCompatibilityHandler.generated.h"




USTRUCT(BlueprintType)
struct FNamedValue
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName InternalName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FDynamicValueStringWrapper Value = FDynamicValueStringWrapper();

	FName GetInternalName() const
	{
		if(InternalName.IsNone()) return FName(FGuid::NewGuid().ToString());
		return InternalName;
	}

	FNamedValue(FString Name, FName InternalName, const FDynamicValueStringWrapper& Value)
	{
		this->Name = Name;
		this->InternalName = InternalName;
		this->Value = Value;
	}

	FNamedValue(){}

	bool operator ==(const FNamedValue& Other) const { return Name == Other.Name && InternalName == Other.InternalName && Value.Type == Other.Value.Type; }
};


UCLASS()
class UBackwardsCompatibilityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure)
	static bool CustomStructReplicatableEqual(const FString& NameOriginal, const TArray<FNamedDynamicValue>& ValuesOriginal, const FString& NameNew, const TArray<FNamedValue>& ValuesNew)
	{
		if(NameOriginal != NameNew) return false;
		return NamedValuesEqual_NoValue(ValuesOriginal, ValuesNew);
	}

	UFUNCTION(BlueprintPure)
	static bool NamedValuesEqual(const TArray<FNamedDynamicValue>& ValuesOriginal, const TArray<FNamedValue>& ValuesNew)
	{
		if(ValuesOriginal.Num() != ValuesNew.Num()) return false;
		for(int i = 0; i < ValuesNew.Num(); i++)
		{
			if(ValuesOriginal[i].Name != ValuesNew[i].Name) return false;
			if(!ValuesOriginal[i].Value || ValuesOriginal[i].Value->ToWrapperValue() != ValuesNew[i].Value) return false;
		}

		return true;
	}

	UFUNCTION(BlueprintPure, DisplayName="Named Values Equal (by type)")
	static bool NamedValuesEqual_NoValue(const TArray<FNamedDynamicValue>& ValuesOriginal, const TArray<FNamedValue>& ValuesNew)
	{
		if(ValuesOriginal.Num() != ValuesNew.Num()) return false;
		for(int i = 0; i < ValuesNew.Num(); i++)
		{
			if(ValuesOriginal[i].Name != ValuesNew[i].Name) return false;
			if(!ValuesOriginal[i].Value || ValuesOriginal[i].Value->ConnectionType != ValuesNew[i].Value.Type) return false;
		}

		return true;
	}

	UFUNCTION(BlueprintCallable)
	static bool ContainsBadFields(const TArray<FNamedValue>& Values)
	{
		TArray<FString> UniqueNames;

		for (const FNamedValue& Value : Values)
		{
			if(Value.Name.Len() == 0) return true;
			if(UniqueNames.Contains(Value.Name)) return true;

			UniqueNames.Add(Value.Name);
		}

		return false;
	}

	UFUNCTION(BlueprintPure)
	static TArray<FNamedValue> SortValues(const TArray<FNamedValue>& Values)
	{
		auto Out = Values;
		Out.StableSort([](const FNamedValue& Lhs, const FNamedValue& Rhs) -> bool
		{
			auto LeftType = Lhs.Value.Type;
			auto RightType = Rhs.Value.Type;

			if(LeftType != RightType) return LeftType < RightType;
			else return Lhs.Name < Rhs.Name;
		});
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static TArray<FNamedValue> SortByType(const TArray<FNamedValue>& Values)
	{
		auto Out = Values;
		Out.StableSort([](const FNamedValue& Lhs, const FNamedValue& Rhs) -> bool
		{
			return Lhs.Value.Type < Rhs.Value.Type;
		});
		
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static TArray<FNamedValue> SortByName(const TArray<FNamedValue>& Values)
	{
		auto Out = Values;
		Out.StableSort([](const FNamedValue& Lhs, const FNamedValue& Rhs) -> bool
		{
			return Lhs.Name < Rhs.Name;
		});
		
		return Out;
	}

	UFUNCTION(BlueprintPure)
	static FNamedValue ToClientValue(UPARAM(ref) FNamedDynamicValue& Value)
	{
		auto ValueWrapper = Value.Value ? Value.Value->ToWrapperValue() : FDynamicValueStringWrapper();
		return FNamedValue(Value.Name, Value.GetInternalName(), ValueWrapper);
	}
};
