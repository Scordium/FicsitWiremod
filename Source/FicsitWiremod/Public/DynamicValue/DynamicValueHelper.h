// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BoolArrayDynamicValue.h"
#include "BoolDynamicValue.h"
#include "ColorArrayDynamicValue.h"
#include "ColorDynamicValue.h"
#include "EntityArrayDynamicValue.h"
#include "EntityDynamicValue.h"
#include "InventoryArrayDynamicValue.h"
#include "InventoryDynamicValue.h"
#include "ItemStackArrayDynamicValue.h"
#include "ItemStackDynamicValue.h"
#include "NumberArrayDynamicValue.h"
#include "NumberDynamicValue.h"
#include "PowerGridArrayDynamicValue.h"
#include "PowerGridDynamicValue.h"
#include "StringArrayDynamicValue.h"
#include "StringDynamicValue.h"
#include "VectorArrayDynamicValue.h"
#include "VectorDynamicValue.h"
#include "DynamicValueBase.h"
#include "DynamicValueHelper.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UDynamicValueHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static UBoolArrayDynamicValue* MakeBoolArrayValue(TArray<bool> value)
	{
		auto newObj = NewObject<UBoolArrayDynamicValue>();
		newObj->ValueType = ArrayOfBoolean;
		newObj->Value = value;

		return newObj;
	}
	
	UFUNCTION(BlueprintCallable)
	static UBoolDynamicValue* MakeBoolValue(bool value)
	{
		auto newObj = NewObject<UBoolDynamicValue>();
		newObj->ValueType = Boolean;
		newObj->Value = value;

		return newObj;
	}
	
	UFUNCTION(BlueprintCallable)
	static UColorArrayDynamicValue* MakeColorArrayValue(TArray<FLinearColor> value)
	{
		auto newObj = NewObject<UColorArrayDynamicValue>();
		newObj->ValueType = ArrayOfColor;
		newObj->Value = value;

		return newObj;
	}

	UFUNCTION(BlueprintCallable)
	static UColorDynamicValue* MakeColorValue(FLinearColor value)
	{
		auto newObj = NewObject<UColorDynamicValue>();
		newObj->ValueType = Color;
		newObj->Value = value;

		return newObj;
	}

	UFUNCTION(BlueprintCallable)
	static UEntityArrayDynamicValue* MakeEntityArrayValue(TArray<AActor*> value)
	{
		auto newObj = NewObject<UEntityArrayDynamicValue>();
		newObj->ValueType = ArrayOfEntity;
		newObj->Value = value;

		return newObj;
	}

	UFUNCTION(BlueprintCallable)
	static UEntityDynamicValue* MakeEntityValue(AActor* value)
	{
		auto newObj = NewObject<UEntityDynamicValue>();
		newObj->ValueType = Entity;
		newObj->Value = value;

		return newObj;
	}

	UFUNCTION(BlueprintCallable)
	static UInventoryArrayDynamicValue* MakeInventoryArrayValue(TArray<UFGInventoryComponent*> value)
	{
		auto newObj = NewObject<UInventoryArrayDynamicValue>();
		newObj->ValueType = ArrayOfInventory;
		newObj->Value = value;

		return newObj;
	}

	UFUNCTION(BlueprintCallable)
	static UInventoryDynamicValue* MakeInventoryValue(UFGInventoryComponent* value)
	{
		auto newObj = NewObject<UInventoryDynamicValue>();
		newObj->ValueType = Inventory;
		newObj->Value = value;

		return newObj;
	}

	UFUNCTION(BlueprintCallable)
	static UItemStackArrayDynamicValue* MakeItemStackArrayValue(TArray<FInventoryStack> value)
	{
		auto newObj = NewObject<UItemStackArrayDynamicValue>();
		newObj->ValueType = ArrayOfStack;
		newObj->Value = value;

		return newObj;
	}
	
	UFUNCTION(BlueprintCallable)
	static UItemStackDynamicValue* MakeItemStackValue(FInventoryStack value)
	{
		auto newObj = NewObject<UItemStackDynamicValue>();
		newObj->ValueType = Stack;
		newObj->Value = value;

		return newObj;
	}

	UFUNCTION(BlueprintCallable)
	static UNumberArrayDynamicValue* MakeNumberArrayValue(TArray<float> value)
	{
		auto newObj = NewObject<UNumberArrayDynamicValue>();
		newObj->ValueType = ArrayOfNumber;
		newObj->Value = value;

		return newObj;
	}

	UFUNCTION(BlueprintCallable)
	static UNumberDynamicValue* MakeNumberValue(float value)
	{
		auto newObj = NewObject<UNumberDynamicValue>();
		newObj->ValueType = Number;
		newObj->Value = value;

		return newObj;
	}

	UFUNCTION(BlueprintCallable)
	static UPowerGridArrayDynamicValue* MakePowerGridArrayValue(TArray<UFGPowerCircuit*> value)
	{
		auto newObj = NewObject<UPowerGridArrayDynamicValue>();
		newObj->ValueType = ArrayOfPowerGrid;
		newObj->Value = value;

		return newObj;
	}
	
	UFUNCTION(BlueprintCallable)
	static UPowerGridDynamicValue* MakePowerGridValue(UFGPowerCircuit* value)
	{
		auto newObj = NewObject<UPowerGridDynamicValue>();
		newObj->ValueType = PowerGrid;
		newObj->Value = value;

		return newObj;
	}

	UFUNCTION(BlueprintCallable)
	static UStringArrayDynamicValue* MakeStringArrayValue(TArray<FString> value)
	{
		auto newObj = NewObject<UStringArrayDynamicValue>();
		newObj->ValueType = ArrayOfString;
		newObj->Value = value;

		return newObj;
	}
	
	UFUNCTION(BlueprintCallable)
	static UStringDynamicValue* MakeStringValue(FString value)
	{
		auto newObj = NewObject<UStringDynamicValue>();
		newObj->ValueType = String;
		newObj->Value = value;

		return newObj;
	}


	UFUNCTION(BlueprintCallable)
	static UVectorArrayDynamicValue* MakeVectorArrayValue(TArray<FVector> value)
	{
		auto newObj = NewObject<UVectorArrayDynamicValue>();
		newObj->ValueType = ArrayOfVector;
		newObj->Value = value;

		return newObj;
	}

	UFUNCTION(BlueprintCallable)
	static UVectorDynamicValue* MakeVectorValue(FVector value)
	{
		auto newObj = NewObject<UVectorDynamicValue>();
		newObj->ValueType = Vector;
		newObj->Value = value;

		return newObj;
	}
};
