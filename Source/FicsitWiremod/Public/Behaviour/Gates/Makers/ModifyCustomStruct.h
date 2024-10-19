// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueUtils.h"
#include "FGWiremodBuildable.h"

#include "ModifyCustomStruct.generated.h"

UCLASS()
class FICSITWIREMOD_API AModifyCustomStruct : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (!IsConnected(0)) return;
		
		auto Struct = GetConnection(0).GetCustomStruct();
		if (StructCache != Struct)
		{
			StructCache = Struct;
			Out = StructCache.Duplicate(this);
		}

		for (int i = 0; i < Out.Values.Num(); i++)
		{
			if (!IsConnected(i + 1)) continue;
			auto& Val = Out.Values[i];

			Val.Value = UCCDynamicValueUtils::FromValue(GetConnection(i + 1), Val.Value);
		}
	}

	virtual TArray<FBuildingConnection> GetConnections_Implementation(EConnectionDirection direction) override
	{
		if (direction == Input)
		{
			auto Output = TArray{CustomStructInputConnection };

			for (const FNamedDynamicValue& Value : Out.Values)
			{
				Output.Add(FBuildingConnection(Value.Name, "", Value.Value->ConnectionType));
			}

			return Output;
		}
		else return Super::GetConnections_Implementation(direction);
	}

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, StructCache)
		DOREPLIFETIME(ThisClass, Out)
	}

	UPROPERTY(EditDefaultsOnly)
	FBuildingConnection CustomStructInputConnection = FBuildingConnection("Input struct", "", EConnectionType::CustomStruct);

	UPROPERTY(Replicated)
	FCustomStruct StructCache;

	UPROPERTY(Replicated)
	FCustomStruct Out;
};
