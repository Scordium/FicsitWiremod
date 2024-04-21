// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "MakeItemStack.generated.h"

UCLASS()
class FICSITWIREMOD_API AMakeItemStack : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto ItemClass = GetConnection(0).GetItemDescriptor();

		Out.Item.SetItemClass(ItemClass);
		Out.NumItems = GetConnection(1).GetFloat();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AMakeItemStack, Out)
	}
	
	UPROPERTY(Replicated, SaveGame)
	FInventoryStack Out;
};
