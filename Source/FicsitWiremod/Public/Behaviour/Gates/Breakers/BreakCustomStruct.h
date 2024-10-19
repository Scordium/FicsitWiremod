// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CustomStruct.h"
#include "BreakCustomStruct.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakCustomStruct : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		CacheValues(GetConnection(0).GetCustomStruct());
	}


	void CacheValues(const FCustomStruct& NewStruct)
	{
		for(auto& Value : NewStruct.Values)
		{
			Cached.Add(Value.Name, Value.Value);
		}
		
		if(NewStruct == Saved) return;
		Saved = NewStruct;
		Cached.Empty();
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override
	{
		if(Cached.Contains(ValueName))
			return Cached[ValueName];

		return nullptr;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakCustomStruct, Saved)
	}

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& OutObjects) override
	{
		Super::GatherReplicatedObjects_Implementation(OutObjects);
		for(auto& Val : Saved.Values)
			OutObjects.Add(Val.Value);
	}

	virtual TArray<FBuildingConnection> GetConnections_Implementation(EConnectionDirection direction) override
	{
		if(direction == Input) return Super::GetConnections_Implementation(direction);
		else
		{
			auto Out = TArray<FBuildingConnection>();
			for(auto& Value : Saved.Values)
			{
				Out.Add(FBuildingConnection(Value.Name, Value.Name, Value.Value->ConnectionType));
			}

			return Out;
		}
	}

	UPROPERTY(Replicated, SaveGame)
	FCustomStruct Saved;

	UPROPERTY()
	TMap<FString, UCCDynamicValueBase*> Cached;
};
