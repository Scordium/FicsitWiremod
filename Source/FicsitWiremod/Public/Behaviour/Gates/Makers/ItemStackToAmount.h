// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ItemStackToAmount.generated.h"

UCLASS()
class FICSITWIREMOD_API AItemStackToAmount : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		const auto Stack = GetConnection(0).GetStack();

		Out.ItemClass = Stack.Item.GetItemClass();
		Out.Amount = Stack.NumItems;
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AItemStackToAmount, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FItemAmount Out;
};
