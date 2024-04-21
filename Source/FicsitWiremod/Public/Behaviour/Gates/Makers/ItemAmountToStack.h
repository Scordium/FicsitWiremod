// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ItemAmountToStack.generated.h"

UCLASS()
class FICSITWIREMOD_API AItemAmountToStack : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto ItemAmount = GetConnection(0).GetItemAmount();

		Out.Item.SetItemClass(ItemAmount.ItemClass);
		Out.NumItems = ItemAmount.Amount;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AItemAmountToStack, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FInventoryStack Out;
};
