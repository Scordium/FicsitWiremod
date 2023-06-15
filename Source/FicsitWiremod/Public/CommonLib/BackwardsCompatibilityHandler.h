// 

#pragma once

#include "CoreMinimal.h"
#include "FGInventoryComponent.h"
#include "FGPowerCircuit.h"
#include "FGRecipe.h"
#include "CommonLib/ConnectionType.h"
#include "DynamicValues/CCBoolArrayValue.h"
#include "DynamicValues/CCBoolValue.h"
#include "DynamicValues/CCCircuitArrayValue.h"
#include "DynamicValues/CCCircuitValue.h"
#include "DynamicValues/CCColorArrayValue.h"
#include "DynamicValues/CCColorValue.h"
#include "DynamicValues/CCDynamicValueBase.h"
#include "DynamicValues/CCEntityArrayValue.h"
#include "DynamicValues/CCEntityValue.h"
#include "DynamicValues/CCIntegerValue.h"
#include "DynamicValues/CCInventoryArrayValue.h"
#include "DynamicValues/CCInventoryValue.h"
#include "DynamicValues/CCItemAmountArrayValue.h"
#include "DynamicValues/CCItemAmountValue.h"
#include "DynamicValues/CCNumberArrayValue.h"
#include "DynamicValues/CCNumberValue.h"
#include "DynamicValues/CCRecipeArrayValue.h"
#include "DynamicValues/CCRecipeValue.h"
#include "DynamicValues/CCStackArrayValue.h"
#include "DynamicValues/CCStackValue.h"
#include "DynamicValues/CCStringArrayValue.h"
#include "DynamicValues/CCStringValue.h"
#include "DynamicValues/CCVectorArrayValue.h"
#include "DynamicValues/CCVectorValue.h"
#include "BackwardsCompatibilityHandler.generated.h"


USTRUCT(BlueprintType)
struct FDynamicValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EConnectionType> ConnectionType = Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool StoredBool = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<bool> BoolArr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float StoredFloat = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<float> NumberArr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString StoredString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FString> StringArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FVector StoredVector = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FVector> VectorArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLinearColor StoredColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FLinearColor> ColorArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, NotReplicated)
	FInventoryStack Stack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, NotReplicated)
	TArray<FInventoryStack> StackArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFGInventoryComponent* Inventory = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UFGInventoryComponent*> InventoryArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFGPowerCircuit* PowerGrid = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UFGPowerCircuit*> PowerGridArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	AActor* Entity = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<AActor*> EntityArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TSubclassOf<UFGRecipe> Recipe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray< TSubclassOf<UFGRecipe> > RecipeArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FItemAmount ItemAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FItemAmount> ItemAmountArr;

	FDynamicValue(){}
	
	FDynamicValue(bool Value)
	{
		ConnectionType = Boolean;
		StoredBool = Value;
	}

	FDynamicValue(FString Value)
	{
		ConnectionType = String;
		StoredString = Value;
	}

	FDynamicValue(float Value)
	{
		ConnectionType = Number;
		StoredFloat = Value;
	}
	
	FDynamicValue(int Value)
	{
		ConnectionType = Number;
		StoredFloat = Value;
	}

	FDynamicValue(FVector Value)
	{
		ConnectionType = Vector;
		StoredVector = Value;
	}
	
	FDynamicValue(FLinearColor Value)
	{
		ConnectionType = Color;
		StoredColor = Value;
	}

	FDynamicValue(UCCDynamicValueBase* ReplicatedBase)
	{
		if(!ReplicatedBase) return;
		ConnectionType = ReplicatedBase->ConnectionType;
		switch (ReplicatedBase->ConnectionType)
		{
		case Boolean: StoredBool = Cast<UCCBoolValue>(ReplicatedBase)->Value; break;
		case Integer: StoredFloat = Cast<UCCIntegerValue>(ReplicatedBase)->Value; break;
		case Number: StoredFloat = Cast<UCCNumberValue>(ReplicatedBase)->Value; break;
		case String: StoredString = Cast<UCCStringValue>(ReplicatedBase)->Value; break;
		case Vector: StoredVector = Cast<UCCVectorValue>(ReplicatedBase)->Value; break;
		case Color: StoredColor = Cast<UCCColorValue>(ReplicatedBase)->Value; break;
		case EConnectionType::Recipe: Recipe = Cast<UCCRecipeValue>(ReplicatedBase)->Value; break;
		default: break;
		}
	}

	UCCDynamicValueBase* Convert(UObject* WorldContext) const
	{
		auto Level = WorldContext->GetWorld()->PersistentLevel;
		switch (ConnectionType)
		{
		case EConnectionType::Boolean:
			{
				auto Object = NewObject<UCCBoolValue>(Level);
				Object->Value = StoredBool;
				return Object;
			}
		case EConnectionType::Color:
			{
				auto Object = NewObject<UCCColorValue>(Level);
				Object->Value = StoredColor;
				return Object;
			}
		case EConnectionType::Entity:
			{
				auto Object = NewObject<UCCEntityValue>(Level);
				Object->Value = Entity;
				return Object;
			}
		case EConnectionType::Inventory:
			{
				auto Object = NewObject<UCCInventoryValue>(Level);
				Object->Value = Inventory;
				return Object;
			}
		case Integer:
		case EConnectionType::Number:
			{
				auto Object = NewObject<UCCNumberValue>(Level);
				Object->Value = StoredFloat;
				return Object;
			}
		case EConnectionType::Recipe:
			{
				auto Object = NewObject<UCCRecipeValue>(Level);
				Object->Value = Recipe;
				return Object;
			}
		case EConnectionType::Stack:
			{
				auto Object = NewObject<UCCStackValue>(Level);
				Object->Value = Stack;
				return Object;
			}
		case EConnectionType::String:
			{
				auto Object = NewObject<UCCStringValue>(Level);
				Object->Value = StoredString;
				return Object;
			}
		case EConnectionType::Vector:
			{
				auto Object = NewObject<UCCVectorValue>(Level);
				Object->Value = StoredVector;
				return Object;
			}
		case EConnectionType::ItemAmount:
			{
				auto Object = NewObject<UCCItemAmountValue>(Level);
				Object->Value = ItemAmount;
				return Object;
			}
		case EConnectionType::PowerGrid:
			{
				auto Object = NewObject<UCCCircuitValue>(Level);
				Object->Value = PowerGrid;
				return Object;
			}
		case EConnectionType::ArrayOfBoolean:
			{
				auto Object = NewObject<UCCBoolArrayValue>(Level);
				Object->Value = BoolArr;
				return Object;
			}
		case EConnectionType::ArrayOfColor:
			{
				auto Object = NewObject<UCCColorArrayValue>(Level);
				Object->Value = ColorArr;
				return Object;
			}
		case EConnectionType::ArrayOfEntity:
			{
				auto Object = NewObject<UCCEntityArrayValue>(Level);
				Object->Value = EntityArr;
				return Object;
			}
		case EConnectionType::ArrayOfInventory:
			{
				auto Object = NewObject<UCCInventoryArrayValue>(Level);
				Object->Value = InventoryArr;
				return Object;
			}
		case EConnectionType::ArrayOfNumber:
			{
				auto Object = NewObject<UCCNumberArrayValue>(Level);
				Object->Value = NumberArr;
				return Object;
			}
		case EConnectionType::ArrayOfRecipe:
			{
				auto Object = NewObject<UCCRecipeArrayValue>(Level);
				Object->Value = RecipeArr;
				return Object;
			}
		case EConnectionType::ArrayOfStack:
			{
				auto Object = NewObject<UCCStackArrayValue>(Level);
				Object->Value = StackArr;
				return Object;
			}
		case EConnectionType::ArrayOfString:
			{
				auto Object = NewObject<UCCStringArrayValue>(Level);
				Object->Value = StringArr;
				return Object;
			}
		case EConnectionType::ArrayOfVector:
			{
				auto Object = NewObject<UCCVectorArrayValue>(Level);
				Object->Value = VectorArr;
				return Object;
			}
		case EConnectionType::ArrayOfItemAmount:
			{
				auto Object = NewObject<UCCItemAmountArrayValue>(Level);
				Object->Value = ItemAmountArr;
				return Object;
			}
		case EConnectionType::ArrayOfPowerGrid:
			{
				auto Object = NewObject<UCCCircuitArrayValue>(Level);
				Object->Value = PowerGridArr;
				return Object;
			}
		default: return nullptr;
		}
	}

	
};

USTRUCT(BlueprintType)
struct FNamedValue
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FDynamicValue Value = FDynamicValue();

	FNamedValue(FString Name, FDynamicValue Value)
	{
		this->Name = Name;
		this->Value = Value;
	}

	FNamedValue(){}

	operator FDynamicValue() {return this->Value; }

	bool operator ==(const FNamedValue& Other)
	{
		return Name == Other.Name && Value.ConnectionType == Other.Value.ConnectionType;
	}

	bool operator !=(const FNamedValue& Other) { return !(*this == Other); }
};


UCLASS()
class UBackwardsCompatibilityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static bool IsValueEqual(const FDynamicValue& Value, UCCDynamicValueBase* OtherValue)
	{
		if(!OtherValue) return false;

		auto Converted = Value.Convert(OtherValue);
		if(!Converted) return false;
		return Converted->Equals(OtherValue);
	}

	UFUNCTION(BlueprintPure)
	static bool CustomStructReplicatableEqual(FString NameOriginal, TArray<FNamedDynamicValue> ValuesOriginal, FString NameNew, TArray<FNamedValue> ValuesNew, UObject* WorldContext)
	{
		if(NameOriginal != NameNew) return false;
		return NamedValuesEqual(ValuesOriginal, ValuesNew, WorldContext);
	}

	UFUNCTION(BlueprintPure)
	static bool NamedValuesEqual(TArray<FNamedDynamicValue> ValuesOriginal, TArray<FNamedValue> ValuesNew, UObject* WorldContext)
	{
		if(ValuesOriginal.Num() != ValuesNew.Num()) return false;
		for(int i = 0; i < ValuesNew.Num(); i++)
		{
			if(ValuesOriginal[i].Name != ValuesNew[i].Name) return false;
			if(!ValuesOriginal[i] || !ValuesOriginal[i].Value->Equals(ValuesNew[i].Value.Convert(WorldContext))) return false;
		}

		return true;
	}

	UFUNCTION(BlueprintPure)
	static FNamedValue ToClientValue(const FNamedDynamicValue& Value) { return FNamedValue(Value.Name, FDynamicValue(Value.Value)); }
};
