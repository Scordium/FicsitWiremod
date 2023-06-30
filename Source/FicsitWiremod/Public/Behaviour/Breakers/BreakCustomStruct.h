// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "BreakCustomStruct.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakCustomStruct : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		CacheValues(GetConnection(0).GetCustomStruct());
	}


	void CacheValues(const FCustomStruct& NewStruct)
	{
		for(auto Value : NewStruct.Values)
		{
			Cached.Add(Value.Name, Value.Value);
		}
		
		if(NewStruct == Saved) return;
		Saved = NewStruct;
		
		Cached.Empty();
		ConnectionsInfo.Outputs.Empty();

		for(auto Value : Saved.Values)
		{
			auto Connection = FBuildingConnection(Value.Name, Value.Name, Value.Value->ConnectionType);
			ConnectionsInfo.Outputs.Add(Connection);
		}
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

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override
	{
		bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
		
		for(auto Val : Saved.Values)
			WroteSomething |= Channel->ReplicateSubobject(Val.Value, *Bunch, *RepFlags);

		return WroteSomething;
	}

	UPROPERTY(Replicated)
	FCustomStruct Saved;

	UPROPERTY()
	TMap<FString, UCCDynamicValueBase*> Cached;
};
