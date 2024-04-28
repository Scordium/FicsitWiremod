// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "ItemAmountToStack.generated.h"

UCLASS()
class FICSITWIREMOD_API AItemAmountToStack : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		//0 - Default mode, single struct
		if(CurrentStateIndex == 0)
		{
			auto ItemAmount = GetConnection(0).GetItemAmount();

			Out.Item.SetItemClass(ItemAmount.ItemClass);
			Out.NumItems = ItemAmount.Amount;
		}
		//1 - Alt mode, struct array
		else if(CurrentStateIndex == 1)
		{
			auto ItemAmounts = GetConnection(0).GetItemAmountArray();
			OutArray.SetNum(ItemAmounts.Num());

			for(int i = 0; i < ItemAmounts.Num(); i++)
			{
				OutArray[i].Item.SetItemClass(ItemAmounts[i].ItemClass);
				OutArray[i].NumItems = ItemAmounts[i].Amount;
			}
		}
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		if(Index == 0) OutArray.Empty();
		else if (Index == 1) Out = FInventoryStack();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AItemAmountToStack, Out)
		DOREPLIFETIME(AItemAmountToStack, OutArray)
	}

	UPROPERTY(Replicated, SaveGame)
	FInventoryStack Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<FInventoryStack> OutArray;
};
