// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/InventoryUtilities.h"
#include "CombineItemStacks.generated.h"

UCLASS()
class FICSITWIREMOD_API ACombineItemStacks : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = UInventoryUtilities::CombineStacks(GetConnection(0).GetStackArray());
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ACombineItemStacks, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	TArray<FInventoryStack> Out;
};
