// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "Transistor.generated.h"

UCLASS()
class FICSITWIREMOD_API ATransistor : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		if(GetConnection(2).GetBool()) Out = nullptr;
		else if(GetConnection(1).GetBool())
			Out = UCCDynamicValueUtils::FromValue(GetConnection(0), Out ? Out->GetWorld() : GetWorld());

		SetOutputType(0, Out ? Out->ConnectionType : Unknown);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ATransistor, Out)
	}

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override
	{
		bool Idk = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

		Channel->ReplicateSubobject(Out, *Bunch, *RepFlags);

		return Idk;
	}


	virtual UCCDynamicValueBase* GetValue_Implementation(const FString& ValueName) override{ return Out; }

	UPROPERTY(Replicated, SaveGame)
	UCCDynamicValueBase* Out;
};
