// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "WiremodUtils.h"
#include "Kismet/KismetArrayLibrary.h"
#include "WiremodArrayFuncs.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiremodArrayFuncs : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	

	UFUNCTION(BlueprintCallable)
	static FDynamicValue GetArrayElement(const FNewConnectionData& data, int index)
	{
		auto out = FDynamicValue();
		out.ConnectionType = UWiremodUtils::ArrayToBase(data.ConnectionType);
		FDynamicValue arrayData;
		UWiremodReflection::FillDynamicStructFromData(data, arrayData);
		switch (data.ConnectionType)
		{
		case Unknown: break;
		case ArrayOfBoolean:
			if(arrayData.BoolArr.IsValidIndex(index))
				out.StoredBool = arrayData.BoolArr[index];
			break;
		
		case ArrayOfNumber:
			if(arrayData.NumberArr.IsValidIndex(index))
				out.StoredFloat = arrayData.NumberArr[index];
			break;

		case ArrayOfString:
			if(arrayData.StringArr.IsValidIndex(index))
				out.StoredString = arrayData.StringArr[index];
			break;

		case ArrayOfVector:
			if(arrayData.VectorArr.IsValidIndex(index))
				out.StoredVector = arrayData.VectorArr[index];
			break;

		case ArrayOfColor:
			if(arrayData.ColorArr.IsValidIndex(index))
				out.StoredColor = arrayData.ColorArr[index];
			break;

		case ArrayOfEntity:
			if(arrayData.EntityArr.IsValidIndex(index))
				out.Entity = arrayData.EntityArr[index];
			break;

		case ArrayOfInventory:
			if(arrayData.InventoryArr.IsValidIndex(index))
				out.Inventory = arrayData.InventoryArr[index];
			break;

		case ArrayOfStack:
			if(arrayData.StackArr.IsValidIndex(index))
				out.Stack = arrayData.StackArr[index];
			break;

		case ArrayOfPowerGrid:
			if(arrayData.PowerGridArr.IsValidIndex(index))
				out.PowerGrid = arrayData.PowerGridArr[index];
			break;

		case ArrayOfRecipe:
			if(arrayData.RecipeArr.IsValidIndex(index))
				out.Recipe = arrayData.RecipeArr[index];
			break;

		case ArrayOfItemAmount:
			if(arrayData.ItemAmountArr.IsValidIndex(index))
				out.ItemAmount = arrayData.ItemAmountArr[index];
			break;
		
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function GET_ARRAY_ELEMENT"), (int)out.ConnectionType);
			break;
		}

		return out;
	}


	UFUNCTION(BlueprintCallable)
	static int ArrayLength(const FNewConnectionData& data)
	{
		switch (data.ConnectionType)
		{
		case Unknown: return 0;
		case ArrayOfBoolean: return UWiremodReflection::GetBoolArray(data).Num();
		case ArrayOfNumber: return UWiremodReflection::GetNumberArray(data).Num();
		case ArrayOfString: return UWiremodReflection::GetStringArray(data).Num();
		case ArrayOfVector: return UWiremodReflection::GetVectorArray(data).Num();
		case ArrayOfColor: return UWiremodReflection::GetColorArray(data).Num();
		case ArrayOfEntity: return UWiremodReflection::GetEntityArray(data).Num();
		case ArrayOfInventory: return UWiremodReflection::GetInventoryArray(data).Num();
		case ArrayOfStack: return UWiremodReflection::GetItemStackArray(data).Num();
		case ArrayOfPowerGrid: return UWiremodReflection::GetPowerGridArray(data).Num();
		case ArrayOfRecipe: return UWiremodReflection::GetRecipeArray(data).Num();
		case ArrayOfItemAmount: return UWiremodReflection::GetItemAmountArray(data).Num();
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function ARRAY_LENGTH. This should not be happening. Possibly trying to use the function outside array-focused gates?"), (int)data.ConnectionType);
			return 0;
		}
	}

	UFUNCTION(BlueprintCallable)
	static void ResizeArray(const FDynamicValue& original, int newSize, FDynamicValue& out)
	{
		out = original;
		
		switch (out.ConnectionType)
		{
		case Unknown: break;
		case ArrayOfBoolean: out.BoolArr.SetNum(newSize); break;
		case ArrayOfNumber: out.NumberArr.SetNum(newSize); break;
		case ArrayOfString: out.StringArr.SetNum(newSize); break;
		case ArrayOfVector: out.VectorArr.SetNum(newSize); break;
		case ArrayOfColor: out.ColorArr.SetNum(newSize); break;
		case ArrayOfEntity: out.EntityArr.SetNum(newSize); break;
		case ArrayOfInventory: out.InventoryArr.SetNum(newSize); break;
		case ArrayOfStack: out.StackArr.SetNum(newSize); break;
		case ArrayOfPowerGrid: out.PowerGridArr.SetNum(newSize); break;
		case ArrayOfRecipe: out.RecipeArr.SetNum(newSize); break;
		case ArrayOfItemAmount: out.ItemAmountArr.SetNum(newSize); break;
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function RESIZE_ARRAY. This should not be happening. Possibly trying to use the function outside array-focused gates?"), (int)out.ConnectionType);
			break;
		}
	}


	UFUNCTION(BlueprintCallable)
	static void SetArrayElem(const FDynamicValue& original, int index, const FNewConnectionData& elem, FDynamicValue& out)
	{
		out = original;
		switch (out.ConnectionType)
		{
		case Unknown: break;
		case ArrayOfBoolean:
			if(out.BoolArr.IsValidIndex(index))
				out.BoolArr[index] = UWiremodReflection::GetFunctionBoolResult(elem); break;
			
		case ArrayOfNumber:
			if(out.NumberArr.IsValidIndex(index))
				out.NumberArr[index] = UWiremodReflection::GetFunctionNumberResult(elem); break;
			
		case ArrayOfString:
			if(out.StringArr.IsValidIndex(index))
				out.StringArr[index] = UWiremodReflection::GetFunctionStringResult(elem); break;
			
		case ArrayOfColor:
			if(out.ColorArr.IsValidIndex(index))
				out.ColorArr[index] = UWiremodReflection::GetFunctionColorResult(elem); break;
			
		case ArrayOfEntity:
			if(out.EntityArr.IsValidIndex(index))
				out.EntityArr[index] = UWiremodReflection::GetFunctionEntityResult(elem); break;
			
		case ArrayOfVector:
			if(out.VectorArr.IsValidIndex(index))
				out.VectorArr[index] = UWiremodReflection::GetFunctionVectorResult(elem); break;
			
		case ArrayOfStack:
			if(out.StackArr.IsValidIndex(index))
				out.StackArr[index] = UWiremodReflection::GetFunctionStackResult(elem); break;
			
		case ArrayOfInventory:
			if(out.InventoryArr.IsValidIndex(index))
				out.InventoryArr[index] = UWiremodReflection::GetFunctionInventory(elem); break;
			
		case ArrayOfPowerGrid:
			if(out.PowerGridArr.IsValidIndex(index))
				out.PowerGridArr[index] = UWiremodReflection::GetFunctionPowerCircuitResult(elem); break;

		case ArrayOfRecipe:
			if(out.RecipeArr.IsValidIndex(index))
				out.RecipeArr[index] = UWiremodReflection::GetFunctionRecipeResult(elem); break;

		case ArrayOfItemAmount:
			if(out.ItemAmountArr.IsValidIndex(index))
				out.ItemAmountArr[index] = UWiremodReflection::GetItemAmount(elem); break;
			
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function SET_ARRAY_ELEMENT. This should not be happening. Possibly trying to use the function outside array-focused gates?"), (int)out.ConnectionType);
			break;
		}
	}

	UFUNCTION(BlueprintCallable)
	static void AddArrayElem(const FDynamicValue& original, const FNewConnectionData& elem, FDynamicValue& out)
	{
		out = original;

		switch (out.ConnectionType)
		{
		case Unknown: break;
		case ArrayOfBoolean: out.BoolArr.Add(UWiremodReflection::GetFunctionBoolResult(elem)); break;
		case ArrayOfNumber: out.NumberArr.Add(UWiremodReflection::GetFunctionNumberResult(elem)); break;
		case ArrayOfString: out.StringArr.Add(UWiremodReflection::GetFunctionStringResult(elem)); break;
		case ArrayOfColor: out.ColorArr.Add(UWiremodReflection::GetFunctionColorResult(elem)); break;
		case ArrayOfEntity: out.EntityArr.Add(UWiremodReflection::GetFunctionEntityResult(elem)); break;
		case ArrayOfVector: out.VectorArr.Add(UWiremodReflection::GetFunctionVectorResult(elem)); break;
		case ArrayOfStack: out.StackArr.Add(UWiremodReflection::GetFunctionStackResult(elem)); break;
		case ArrayOfInventory: out.InventoryArr.Add(UWiremodReflection::GetFunctionInventory(elem)); break;
		case ArrayOfPowerGrid: out.PowerGridArr.Add(UWiremodReflection::GetFunctionPowerCircuitResult(elem)); break;
		case ArrayOfRecipe: out.RecipeArr.Add(UWiremodReflection::GetFunctionRecipeResult(elem)); break;
		case ArrayOfItemAmount: out.ItemAmountArr.Add(UWiremodReflection::GetItemAmount(elem)); break;
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function ADD_ARRAY_ELEMENT. This should not be happening. Possibly trying to use the function outside array-focused gates?"), (int)out.ConnectionType);
			break;
		}
	}


	UFUNCTION(BlueprintCallable)
	static void RemoveArrayElement(const FDynamicValue& original, const FNewConnectionData& indexData, FDynamicValue& out)
	{
		out = original;
		const int Index = UWiremodReflection::GetFunctionNumberResult(indexData);
		
		switch (out.ConnectionType)
		{
		case Unknown: break;
		case ArrayOfBoolean:
			if(out.BoolArr.IsValidIndex(Index))
				out.BoolArr.RemoveAt(Index); break;
		case ArrayOfNumber:
			if(out.NumberArr.IsValidIndex(Index))
				out.NumberArr.RemoveAt(Index); break;
		case ArrayOfString:
			if(out.StringArr.IsValidIndex(Index))
				out.StringArr.RemoveAt(Index); break;
		case ArrayOfColor:
			if(out.ColorArr.IsValidIndex(Index))
				out.ColorArr.RemoveAt(Index); break;
		case ArrayOfEntity:
			if(out.EntityArr.IsValidIndex(Index))
				out.EntityArr.RemoveAt(Index); break;
		case ArrayOfVector:
			if(out.VectorArr.IsValidIndex(Index))
				out.VectorArr.RemoveAt(Index); break;
		case ArrayOfStack:
			if(out.StackArr.IsValidIndex(Index))
				out.StackArr.RemoveAt(Index); break;
		case ArrayOfInventory:
			if(out.InventoryArr.IsValidIndex(Index))
				out.InventoryArr.RemoveAt(Index); break;
		case ArrayOfPowerGrid:
			if(out.PowerGridArr.IsValidIndex(Index))
				out.PowerGridArr.RemoveAt(Index); break;
		case ArrayOfRecipe:
			if(out.RecipeArr.IsValidIndex(Index))
				out.RecipeArr.RemoveAt(Index); break;
		case ArrayOfItemAmount:
			if(out.ItemAmountArr.IsValidIndex(Index))
				out.ItemAmountArr.RemoveAt(Index); break;
			
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function REMOVE_ARRAY_ELEMENT. This should not be happening. Possibly trying to use the function outside array-focused gates?"), (int)out.ConnectionType);
			break;
		}
	}

	UFUNCTION(BlueprintCallable)
	static void InsertArrayElement(const FDynamicValue& original, const FNewConnectionData& element, const FNewConnectionData& indexData, FDynamicValue& out)
	{
		out = original;
		int Index = UWiremodReflection::GetFunctionNumberResult(indexData);
		
		
		switch (out.ConnectionType)
		{
		case Unknown: break;
		case ArrayOfBoolean:
			if(out.BoolArr.IsValidIndex(Index))
				out.BoolArr.Insert(UWiremodReflection::GetFunctionBoolResult(element), Index); break;
		case ArrayOfNumber:
			if(out.NumberArr.IsValidIndex(Index))
				out.NumberArr.Insert(UWiremodReflection::GetFunctionNumberResult(element), Index); break;
		case ArrayOfString:
			if(out.StringArr.IsValidIndex(Index))
				out.StringArr.Insert(UWiremodReflection::GetFunctionStringResult(element), Index); break;
		case ArrayOfColor:
			if(out.ColorArr.IsValidIndex(Index))
				out.ColorArr.Insert(UWiremodReflection::GetFunctionColorResult(element), Index); break;
		case ArrayOfEntity:
			if(out.EntityArr.IsValidIndex(Index))
				out.EntityArr.Insert(UWiremodReflection::GetFunctionEntityResult(element), Index); break;
		case ArrayOfVector:
			if(out.VectorArr.IsValidIndex(Index))
				out.VectorArr.Insert(UWiremodReflection::GetFunctionVectorResult(element), Index); break;
		case ArrayOfStack:
			if(out.StackArr.IsValidIndex(Index))
				out.StackArr.Insert(UWiremodReflection::GetFunctionStackResult(element), Index); break;
		case ArrayOfInventory:
			if(out.InventoryArr.IsValidIndex(Index))
				out.InventoryArr.Insert(UWiremodReflection::GetFunctionInventory(element), Index); break;
		case ArrayOfPowerGrid:
			if(out.PowerGridArr.IsValidIndex(Index))
				out.PowerGridArr.Insert(UWiremodReflection::GetFunctionPowerCircuitResult(element), Index); break;
		case ArrayOfRecipe:
			if(out.RecipeArr.IsValidIndex(Index))
				out.RecipeArr.Insert(UWiremodReflection::GetFunctionRecipeResult(element), Index); break;
		case ArrayOfItemAmount:
			if(out.ItemAmountArr.IsValidIndex(Index))
				out.ItemAmountArr.Insert(UWiremodReflection::GetItemAmount(element), Index); break;
			
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function INSERT_ARRAY_ELEMENT. This should not be happening. Possibly trying to use the function outside array-focused gates?"), (int)out.ConnectionType);
			break;
		}
	}


	UFUNCTION(BlueprintCallable)
	static void MakeArray(TArray<FNewConnectionData> Elements, EConnectionType arrayType, FDynamicValue& out)
	{
		if(Elements.Num() == 0) return;

		out.ConnectionType = arrayType;

		switch (out.ConnectionType)
		{
		case ArrayOfBoolean:
			for (FNewConnectionData element : Elements)
				out.BoolArr.Add(WM::GetFunctionBoolResult(element));
			break;

		case ArrayOfNumber:
			for (FNewConnectionData element : Elements)
				out.NumberArr.Add(WM::GetFunctionNumberResult(element));
			break;

		case ArrayOfString:
			for (FNewConnectionData element : Elements)
				out.StringArr.Add(WM::GetFunctionStringResult(element));
			break;

		case ArrayOfVector:
			for (FNewConnectionData element : Elements)
				out.VectorArr.Add(WM::GetFunctionVectorResult(element));
			break;

		case ArrayOfColor:
			for (FNewConnectionData element : Elements)
				out.ColorArr.Add(WM::GetFunctionColorResult(element));
			break;

		case ArrayOfEntity:
			for (FNewConnectionData element : Elements)
				out.EntityArr.Add(WM::GetFunctionEntityResult(element));
			break;

		case ArrayOfStack:
			for (FNewConnectionData element : Elements)
				out.StackArr.Add(WM::GetFunctionStackResult(element));
			break;

		case ArrayOfInventory:
			for (FNewConnectionData element : Elements)
				out.InventoryArr.Add(WM::GetFunctionInventory(element));
			break;

		case ArrayOfRecipe:
			for (FNewConnectionData element : Elements)
				out.RecipeArr.Add(WM::GetFunctionRecipeResult(element));
			break;
		
		case ArrayOfPowerGrid:
			for (FNewConnectionData element : Elements)
				out.PowerGridArr.Add(WM::GetFunctionPowerCircuitResult(element));
			break;

		case ArrayOfItemAmount:
			for (FNewConnectionData element : Elements)
				out.ItemAmountArr.Add(WM::GetItemAmount(element));
			break;
			
		default: break;
		}
	}
	
};
