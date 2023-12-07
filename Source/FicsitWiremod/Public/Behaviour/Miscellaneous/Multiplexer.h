// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "Multiplexer.generated.h"

UCLASS()
class FICSITWIREMOD_API AMultiplexer : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Connection = GetFirstActive();
		if(Connection.IsValid()) Value = UCCDynamicValueUtils::FromValue(Connection, this);
		SetOutputType(0, Value ? Value->ConnectionType.GetValue() : Unknown);
	}
	
	virtual UObject* GetValue_Implementation(const FString& ValueName) override { return Value; }


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AMultiplexer, Value);
	}

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& Out) override
	{
		Super::GatherReplicatedObjects_Implementation(Out);
		Out.Add(Value);
	}

	virtual TArray<FBuildingConnection> GetConnectionsInfo_Implementation(EConnectionDirection direction, int& Count, FBuildableNote& Note) override
	{
		if(direction == Output) return Super::GetConnectionsInfo_Implementation(direction, Count, Note);
		
		int NumCreate = (InputConnections.Num() + 1) / 2 + 1;
		auto Type = GetFirstConnected().ConnectionType;
		if(Type == Unknown) Type = Any;

		auto Out = TArray<FBuildingConnection>();
		for(int i = 0; i < NumCreate; i++)
		{
			auto ValueConnection = FBuildingConnection(FText::FormatOrdered(ValueIndexFormat, i), "", Type);
			auto SelectConnection = FBuildingConnection(FText::FormatOrdered(SelectIndexFormat, i), "", Boolean);

			Out.Add(ValueConnection);
			Out.Add(SelectConnection);
		}

		return Out;
	}
	
	UPROPERTY(Replicated, SaveGame, SaveGame)
	UCCDynamicValueBase* Value;

	UPROPERTY(EditDefaultsOnly)
	FText ValueIndexFormat = FText::FromString("Value {0}");

	UPROPERTY(EditDefaultsOnly)
	FText SelectIndexFormat = FText::FromString("Select {0}");
	
private:


	FConnectionData GetFirstActive()
	{
		for(int i = 1; i < InputConnections.Num(); i += 2)
		{
			if(GetConnection(i).GetBool()) return GetConnection(i - 1);
		}

		return FConnectionData();
	}

	FConnectionData GetFirstConnected()
	{
		for(int i = 0; i < InputConnections.Num(); i += 2)
		{
			auto Connection = GetConnection(i);
			if(Connection.IsValid()) return Connection;
		}

		return FConnectionData();
	}
};
