// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "RadioTransmitter.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "RadioReceiver.generated.h"

UCLASS()
class FICSITWIREMOD_API ARadioReceiver : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		if(TransmitterReference)
		{
			DataReceived = UCCDynamicValueUtils::FromValue(TransmitterReference->GetConnection(0), DataReceived ? DataReceived->GetWorld() : this->GetWorld());
			SetOutputType(0, DataReceived ? DataReceived->ConnectionType : Unknown);
		}
	}

	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ARadioReceiver, TransmitterReference);
		DOREPLIFETIME(ARadioReceiver, DataReceived);
	}

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override
	{
		bool Idk = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
		
		Channel->ReplicateSubobject(DataReceived, *Bunch, *RepFlags);

		return Idk;
	}


	UFUNCTION(BlueprintCallable)
	void SetTransmitter(ARadioTransmitter* NewTransmitter, UObject* Actor)
	{
		if(!GetCanConfigure(Actor)) return;

		TransmitterReference = NewTransmitter;
	}

	virtual UCCDynamicValueBase* GetValue_Implementation(const FString& ValueName) override{ return DataReceived; }


	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, SaveGame)
	ARadioTransmitter* TransmitterReference;

	UPROPERTY(Replicated, VisibleInstanceOnly)
	UCCDynamicValueBase* DataReceived;
};
