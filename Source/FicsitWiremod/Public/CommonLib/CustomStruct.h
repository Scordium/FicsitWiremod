// 

#pragma once

#include "CoreMinimal.h"
#include "DynamicValues/CCDynamicValueBase.h"
#include "CustomStruct.generated.h"

USTRUCT(BlueprintType)
struct FNamedDynamicValue
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FString Name;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	UCCDynamicValueBase* Value = nullptr;

	FNamedDynamicValue(){}
	FNamedDynamicValue(FString Name, UCCDynamicValueBase* Value)
	{
		this->Name = Name;
		this->Value = Value;
	}
	
	bool operator ==(const FNamedDynamicValue& Other) { return Value == Other.Value; }
	bool operator !=(const FNamedDynamicValue& Other) { return Value != Other.Value; }

	operator UCCDynamicValueBase* () { return Value; }
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
};



UCLASS()
class UCustomStructUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintPure)
	static bool CustomStructsEqual(const FCustomStruct& Struct1, const FCustomStruct& Struct2){ return Struct1 == Struct2; }
	
};
