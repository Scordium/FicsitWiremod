// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "ItemStackToAmount.generated.h"

UCLASS()
class FICSITWIREMOD_API AItemStackToAmount : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		//0 - Default mode, single struct
		if(CurrentStateIndex == 0)
		{
			auto ItemStack = GetConnection(0).GetStack();

			Out.ItemClass = ItemStack.Item.GetItemClass();
			Out.Amount = ItemStack.NumItems;
		}
		//1 - Alt mode, struct array
		else if(CurrentStateIndex == 1)
		{
			auto ItemStacks = GetConnection(0).GetStackArray();
			OutArray.SetNum(ItemStacks.Num());

			for(int i = 0; i < ItemStacks.Num(); i++)
			{
				OutArray[i].ItemClass = ItemStacks[i].Item.GetItemClass();
				OutArray[i].Amount = ItemStacks[i].NumItems;
			}
		}
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		if(Index == 0) OutArray.Empty();
		else if (Index == 1) Out = FItemAmount();
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AItemStackToAmount, Out)
		DOREPLIFETIME(AItemStackToAmount, OutArray)
	}

	UPROPERTY(Replicated, SaveGame)
	FItemAmount Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<FItemAmount> OutArray;
};
