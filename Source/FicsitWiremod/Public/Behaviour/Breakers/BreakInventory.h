// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "BreakInventory.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakInventory : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	
	virtual void Process_Implementation(double DeltaTime) override
	{
		Inventory = GetConnection(0).GetInventory();

		if(Inventory)
		{
			if(GetConnection(1).GetBool())
				Inventory->Empty();


			CalculateItemCount();
		}
	}


	UFUNCTION(BlueprintPure)
	bool IsEmpty() const
	{
		if(!Inventory) return false;
		return Inventory->IsEmpty();
	}

	UFUNCTION(BlueprintPure)
	bool CanBeRearranged() const
	{
		if(!Inventory) return false;
		return Inventory->GetCanBeRearranged();
	}

	UFUNCTION(BlueprintPure)
	TArray<FInventoryStack> GetStackArray() const
	{
		TArray<FInventoryStack> Stacks = TArray<FInventoryStack>();
		if(Inventory) Inventory->GetInventoryStacks(Stacks);
		return Stacks;
	}

	UFUNCTION(BlueprintPure)
	int GetStackArrayLength() const
	{
		return GetStackArray().Num();
	}


	//Whether all slots are occupied by an item. Doesn't actually mean it's full.
	UFUNCTION(BlueprintPure)
	bool IsFull() const
	{
		if(!Inventory) return false;
		return !Inventory->HasEnoughSpaceForItem(ItemComparator);
	}

	
	//Item count of all slots combined
	UFUNCTION(BlueprintPure)
	int ItemCount() const
	{
		return ItemCountCache;
	}

	void CalculateItemCount()
	{
		if(!Inventory)
		{
			ItemCountCache = 0;
			return;
		}
        
		int Sum = 0;
		for(const auto Stack : Inventory->mInventoryStacks)
		{
			Sum += Stack.NumItems;
		}
        
		ItemCountCache = Sum;
	}

	UFUNCTION(BlueprintPure)
	int InventorySize() const
	{
		if(!Inventory) return -1;
		return Inventory->GetSizeLinear();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakInventory, Inventory)
	}
	
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Replicated)
	UFGInventoryComponent* Inventory;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FInventoryItem ItemComparator;

	UPROPERTY(VisibleInstanceOnly)
	int ItemCountCache;
};
