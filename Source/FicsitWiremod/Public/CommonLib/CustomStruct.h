// 

#pragma once

#include "CoreMinimal.h"
#include "DynamicValues/CCDynamicValueBase.h"
#include "CustomStruct.generated.h"

USTRUCT(BlueprintType)
struct FNamedDynamicValue
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	UCCDynamicValueBase* Value = nullptr;

	FNamedDynamicValue()
	{
		this->InternalName = FName(FGuid::NewGuid().ToString());
	}
	FNamedDynamicValue(FString Name, UCCDynamicValueBase* Value)
	{
		this->Name = Name;
		this->InternalName = FName(FGuid::NewGuid().ToString());
		this->Value = Value;
	}

	FNamedDynamicValue(FString Name, FName InternalName, UCCDynamicValueBase* Value)
	{
		this->Name = Name;
		this->InternalName = InternalName;
		this->Value = Value;
	}

	FName GetInternalName()
	{
		if(InternalName.IsNone()) InternalName = FName(Name);

		return InternalName;
	}

	void SetInternalName(FString NewInternalName)
	{
		InternalName = FName(NewInternalName);
	}
	
	bool operator ==(const FNamedDynamicValue& Other) const { return Value ? Value->Equals(Other.Value) : (Value == nullptr && Other.Value == nullptr); }

	bool IsDataTypeEqual(const FNamedDynamicValue& Other) const {
		if (!Value || !Other.Value) return false;
		return Name == Other.Name && Other.Value->ConnectionType == Value->ConnectionType;
	}

	operator UCCDynamicValueBase* () const { return Value; }

private:

	/*
	 * Internal name used for things like constant gates to allow having 2 variables with the same name.
	 */
	UPROPERTY(SaveGame)
	FName InternalName = FName(Name);
};

USTRUCT(BlueprintType)
struct FCustomStruct
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FString Name;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TArray<FNamedDynamicValue> Values;

	bool operator ==(const FCustomStruct& CompareTo) const
	{
		if(CompareTo.Name != Name) return false;
		if(CompareTo.Values.Num() != Values.Num()) return false;
		
		for(int i = 0; i < Values.Num() && i < CompareTo.Values.Num(); i++)
		{
			auto A = Values[i];
			auto B = CompareTo.Values[i];

			if(A != B) return false;
		}

		return true;
	}

	bool operator !=(const FCustomStruct& Other) { return !(*this == Other); }
	FCustomStruct Duplicate(UObject* WorldContext);

	TSharedRef<FJsonValue> ToJson() const
	{
		const TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

		JsonObject->SetField(FString("__CIRCUITRY_STRUCT_NAME__"), MakeShareable(new FJsonValueString(Name)));
		for (const FNamedDynamicValue& Value : Values)
		{
			JsonObject->SetField(Value.Name, Value.Value->ToJson());
		}
		
		return MakeShareable(new FJsonValueObject(JsonObject));
	}
};


UCLASS()
class UCustomStructUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintPure)
	static bool CustomStructsEqual(const FCustomStruct& Struct1, const FCustomStruct& Struct2){ return Struct1 == Struct2; }
	
};
