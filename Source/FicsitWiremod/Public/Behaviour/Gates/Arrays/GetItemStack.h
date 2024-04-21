// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "GetItemStack.generated.h"

UCLASS()
class FICSITWIREMOD_API AGetItemStack : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Stacks = GetConnection(0).GetStackArray();
		auto Filter = GetConnection(1).GetItemDescriptor();
		
		//Find stack that matches filter class and set it as output
		for(auto Stack : Stacks)
		{
			if(Stack.Item.GetItemClass()->IsChildOf(Filter))
			{
				Out = Stack;
				return;
			}
		}

		//If nothing was found, use default values
		Out.Item = FInventoryItem::NullInventoryItem;
		Out.NumItems = 0;
	}

	UFUNCTION() TSubclassOf<UFGItemDescriptor> GetDescriptor() const { return Out.Item.GetItemClass(); }
	UFUNCTION() int GetNumItems() const { return Out.NumItems; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AGetItemStack, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FInventoryStack Out;
};
