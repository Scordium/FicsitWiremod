// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCBoolValue.h"
#include "SignalSelector.generated.h"

UCLASS()
class FICSITWIREMOD_API ASignalSelector : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		CurrentSignalValue = GetConnection(0).GetFloat();
		MaxSignalValue = GetConnection(1).GetFloat();
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override
	{
		int SignalSeekValue = FCString::Atoi(*ValueName);
		auto Cache = GetCache();

		Cache->Value = SignalSeekValue == CurrentSignalValue;
		return Cache;
	}

	virtual TArray<FBuildingConnection> GetConnectionsInfo_Implementation(EConnectionDirection Direction, FBuildableNote& Note) override
	{
		TArray<FBuildingConnection> Connections;

		if(Direction == Input) return Super::GetConnectionsInfo_Implementation(Direction, Note);

		for(int i = 0; i < MaxSignalValue; i++)
		{
			auto ConnectionName = FText::FormatOrdered(OutputSignalFormat, i);
			
			Connections.Add(FBuildingConnection(ConnectionName, FString::FromInt(i), Boolean));
		}

		return Connections;
	}

	UCCBoolValue* GetCache()
	{
		if(!SignalValueCache) SignalValueCache = NewObject<UCCBoolValue>(this);

		return SignalValueCache;
	}

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, CurrentSignalValue)
		DOREPLIFETIME(ThisClass, MaxSignalValue)
	}

	UPROPERTY(Replicated)
	int CurrentSignalValue;

	UPROPERTY(Replicated)
	int MaxSignalValue;

	UPROPERTY(EditDefaultsOnly)
	FText OutputSignalFormat = FText::FromString("Signal {0}");

	UPROPERTY()
	UCCBoolValue* SignalValueCache;
};
