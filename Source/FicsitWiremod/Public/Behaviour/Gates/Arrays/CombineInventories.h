// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/InventoryUtilities.h"
#include "CombineInventories.generated.h"

UCLASS()
class FICSITWIREMOD_API ACombineInventories : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Inventories = GetConnection(0).GetInventoryArray();

		TArray<FInventoryStack> OutStacks;
		int OutSlots = 0;
		int OutOccupiedSlots = 0;
		bool OutIsFull = true;

		for (UFGInventoryComponent* Inventory : Inventories)
		{
			if(!Inventory) continue;
			
			TArray<FInventoryStack> Stacks;
			Inventory->GetInventoryStacks(Stacks);

			OutStacks.Append(Stacks);
			OutSlots += Inventory->GetSizeLinear();
			OutOccupiedSlots += Stacks.Num();

			OutIsFull &= !Inventory->HasEnoughSpaceForItem(FullnessCheckItem);
		}

		CombinedStacks = UInventoryUtilities::CombineStacks(OutStacks);
		TotalSlots = OutSlots;
		TotalOccupiedSlots = OutOccupiedSlots;
		IsFull = OutIsFull;

		int OutItemCount = 0;
		for (FInventoryStack& CombinedStack : CombinedStacks)
		{
			OutItemCount += CombinedStack.NumItems;
		}
		TotalItemCount = OutItemCount;
		IsEmpty = TotalItemCount == 0;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ACombineInventories, CombinedStacks)
		DOREPLIFETIME(ACombineInventories, TotalItemCount)
		DOREPLIFETIME(ACombineInventories, TotalSlots)
		DOREPLIFETIME(ACombineInventories, TotalOccupiedSlots)
		DOREPLIFETIME(ACombineInventories, IsFull)
		DOREPLIFETIME(ACombineInventories, IsEmpty)
	}

	UPROPERTY(Replicated, SaveGame)
	TArray<FInventoryStack> CombinedStacks;

	UPROPERTY(Replicated, SaveGame)
	int TotalItemCount;

	UPROPERTY(Replicated, SaveGame)
	int TotalSlots;

	UPROPERTY(Replicated, SaveGame)
	int TotalOccupiedSlots;

	UPROPERTY(Replicated, SaveGame)
	bool IsFull;

	UPROPERTY(Replicated, SaveGame)
	bool IsEmpty;

	UPROPERTY(EditDefaultsOnly)
	FInventoryItem FullnessCheckItem;
};
