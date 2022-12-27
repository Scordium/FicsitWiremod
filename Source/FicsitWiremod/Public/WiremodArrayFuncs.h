// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "WiremodArrayFuncs.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiremodArrayFuncs : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	static EConnectionType BaseToArray(EConnectionType in)
	{
		switch (in)
		{
		case Unknown: return Unknown;
		case Boolean: return ArrayOfBoolean;
		case String: return ArrayOfString;
		case Integer:
		case Number: return ArrayOfNumber;
		case Color: return ArrayOfColor;
		case Vector: return ArrayOfVector;
		case Entity: return ArrayOfEntity;
		case PowerGrid: return ArrayOfPowerGrid;
		case Inventory: return ArrayOfInventory;
		case Stack: return ArrayOfStack;
		case Recipe: return ArrayOfRecipe;
		case Any: return Any;
		case AnyNonArray: return AnyArray;
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function BASE_TO_ARRAY"), in);
			return Unknown;
		}
	}

	UFUNCTION(BlueprintPure)
	static EConnectionType ArrayToBase(EConnectionType in)
	{
		switch (in)
		{
		case Unknown: return Unknown;
		case ArrayOfBoolean: return Boolean;
		case ArrayOfNumber: return Number;
		case ArrayOfString: return String;
		case ArrayOfVector: return Vector;
		case ArrayOfColor: return Color;
		case ArrayOfEntity: return Entity;
		case ArrayOfInventory: return Inventory;
		case ArrayOfStack: return Stack;
		case ArrayOfPowerGrid: return PowerGrid;
		case ArrayOfRecipe: return Recipe;
		case Any: return Any;
		case AnyArray: return AnyNonArray;
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function ARRAY_TO_BASE"), in);
			return Unknown;
		}
	}

	UFUNCTION(BlueprintPure)
	static bool IsArrayType(EConnectionType type)
	{
		switch (type)
		{
		case ArrayOfBoolean:
		case ArrayOfVector:
		case ArrayOfColor:
		case ArrayOfEntity:
		case ArrayOfInventory:
		case ArrayOfNumber:
		case ArrayOfStack:
		case ArrayOfString:
		case ArrayOfPowerGrid:
		case ArrayOfRecipe:
			return true;

		default: return false;
		}
	}

	UFUNCTION(BlueprintCallable)
	static FDynamicValue GetArrayElement(const FNewConnectionData& data, int index)
	{
		auto out = FDynamicValue();
		out.ConnectionType = ArrayToBase(data.ConnectionType);
		FDynamicValue arrayData;
		UWiremodReflection::FillDynamicStructFromData(data, arrayData);
		switch (data.ConnectionType)
		{
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
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function ARRAY_LENGTH. This should not be happening. Possibly trying to use the function outside array-focused gates?"), (int)data.ConnectionType);
			return 0;
		}
	}

	UFUNCTION(BlueprintCallable)
	static void ResizeArray(const FDynamicValue& original, int newSize, FDynamicValue& out)
	{
		out = original;
		
		newSize = UKismetMathLibrary::Clamp(newSize, 0, 100);
		switch (out.ConnectionType)
		{
		case ArrayOfBoolean: out.BoolArr.SetNum(newSize); break;
		case ArrayOfNumber: out.NumberArr.SetNum(newSize); break;
		case ArrayOfString: out.StringArr.SetNum(newSize); break;
		case ArrayOfVector: out.VectorArr.SetNum(newSize); break;
		case ArrayOfColor: out.ColorArr.SetNum(newSize); break;
		case ArrayOfEntity: out.EntityArr.SetNum(newSize); break;
		case ArrayOfInventory: out.InventoryArr.SetNum(newSize); break;
		case ArrayOfStack: out.StackArr.SetNum(newSize); break;
		case ArrayOfPowerGrid: out.PowerGridArr.SetNum(newSize); break;
		case ArrayOfRecipe: out.RecipeArr.SetNum(newSize);
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
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function ADD_ARRAY_ELEMENT. This should not be happening. Possibly trying to use the function outside array-focused gates?"), (int)out.ConnectionType);
			break;
		}
	}
};
