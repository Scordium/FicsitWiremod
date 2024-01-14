// 

#pragma once

#include "CoreMinimal.h"
#include "FGInventoryComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryUtilities.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UInventoryUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*
	 * Combines multiple stacks of the same item into one stack,
	 * Returns array with all similar stacks combined.
	 */
	UFUNCTION(BlueprintCallable)
	static TArray<FInventoryStack> CombineStacks(const TArray<FInventoryStack>& Original)
	{
		TMap<TSubclassOf<UFGItemDescriptor>, int> OutputMap;

		for(auto& Stack : Original)
		{
			auto MapElement = OutputMap.Find(Stack.Item.GetItemClass());
			if(MapElement) *MapElement += Stack.NumItems;
			else OutputMap.Add(Stack.Item.GetItemClass(), Stack.NumItems);
		}

		TArray<FInventoryStack> Output;
		for(const auto& MapElement : OutputMap)
		{
			auto Item = FInventoryStack(MapElement.Value, MapElement.Key);
			Output.Add(Item);
		}

		return Output;
	}

	/*
	 * Checks if 2 stack arrays are equal member-wise
	 */
	UFUNCTION(BlueprintPure, DisplayName="Equal (Inventory Stack Array)", meta=(CompactNodeTitle="=="))
	static bool Equal_InventoryStackArray(const TArray<FInventoryStack>& Arr1, const TArray<FInventoryStack>& Arr2)
	{
		if(Arr1.Num() != Arr2.Num()) return false;

		for(int i = 0; i < Arr1.Num(); i++)
			if(!Equal_InventoryStack(Arr1[i], Arr2[i])) return false;
		
		return true;
	}

	/*
	 * Checks if 2 stacks are equal (amount and class wise)
	 */
	UFUNCTION(BlueprintPure, DisplayName="Equal (Inventory Stack)", meta=(CompactNodeTitle="=="))
	static bool Equal_InventoryStack(const FInventoryStack& A, const FInventoryStack& B)
	{
		return A.NumItems == B.NumItems && A.Item.GetItemClass() == B.Item.GetItemClass();
	}

	/*
	 * Checks if 2 stacks are equal (class wise)
	 */
	UFUNCTION(BlueprintPure, DisplayName="Equal Class (Inventory Stack)", meta=(CompactNodeTitle="<>=="))
	static bool Equal_InventoryStackClass(const FInventoryStack& A, const FInventoryStack& B)
	{
		return A.Item.GetItemClass() == B.Item.GetItemClass();
    }
};
