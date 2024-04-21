// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "MakeItemAmount.generated.h"

UCLASS()
class FICSITWIREMOD_API AMakeItemAmount : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out.ItemClass = GetConnection(0).GetItemDescriptor();
		Out.Amount = GetConnection(1).GetFloat();
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AMakeItemAmount, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FItemAmount Out;
};
