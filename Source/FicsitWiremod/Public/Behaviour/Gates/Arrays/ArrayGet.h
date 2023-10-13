// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ArrayGet.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayGet : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()
    
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Value = UCCDynamicValueUtils::FromValue(GetConnection(0), this);
		if(auto Array = Cast<UCCArrayValueBase>(Value))
		{
			Out = Array->GetElement(GetConnection(1).GetFloat());
		}
		else Out = nullptr;
		
		SetOutputType(0, Out ? Out->ConnectionType.GetValue() : Unknown);
	}

	//Moved implementation out of process function to allow other gates to use this.
	static UCCDynamicValueBase* GetArrayElement(const FConnectionData& Array, const int Index)
	{
		auto Out = UCCDynamicValueUtils::FromValue(Array, Array.Object);
		return Cast<UCCArrayValueBase>(Out)->GetElement(Index);
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayGet, Out);
	}

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override
	{
		bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
		WroteSomething |= Channel->ReplicateSubobject(Out, *Bunch, *RepFlags);

		return WroteSomething;
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override{ return Out; }
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	UCCDynamicValueBase* Out;
};
