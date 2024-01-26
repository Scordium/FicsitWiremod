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
		if(InternalName == "None") return FName(FGuid::NewGuid().ToString());
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
	static bool CustomStructReplicatableEqual(FString NameOriginal, const TArray<FNamedDynamicValue>& ValuesOriginal, FString NameNew, const TArray<FNamedValue>& ValuesNew, UObject* WorldContext)
	{
		if(NameOriginal != NameNew) return false;
		return NamedValuesEqual(ValuesOriginal, ValuesNew, WorldContext);
	}

	UFUNCTION(BlueprintPure)
	static bool NamedValuesEqual(const TArray<FNamedDynamicValue>& ValuesOriginal, const TArray<FNamedValue>& ValuesNew, UObject* WorldContext)
	{
		if(ValuesOriginal.Num() != ValuesNew.Num()) return false;
		for(int i = 0; i < ValuesNew.Num(); i++)
		{
			if(ValuesOriginal[i].Name != ValuesNew[i].Name) return false;
			if(!ValuesOriginal[i].Value || ValuesOriginal[i].Value->ToWrapperValue() != ValuesNew[i].Value) return false;
		}

		return true;
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
