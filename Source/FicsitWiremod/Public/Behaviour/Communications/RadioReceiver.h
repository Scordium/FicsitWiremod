// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "RadioTransmitter.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RadioReceiver.generated.h"

UCLASS()
class FICSITWIREMOD_API ARadioReceiver : public AFGWiremodBuildable, public IIConstantsDistributor
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		if(TransmitterReference)
		{
			WM::FillDynamicStructFromData(TransmitterReference->GetConnection(0), DataReceived);
			SetOutputType(0, DataReceived.ConnectionType);
		}
	}

	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ARadioReceiver, TransmitterReference);
		DOREPLIFETIME(ARadioReceiver, DataReceived);
	}


	UFUNCTION(BlueprintCallable)
	void SetTransmitter(ARadioTransmitter* NewTransmitter, UObject* Actor)
	{
		if(!GetCanConfigure(Actor)) return;

		TransmitterReference = NewTransmitter;
	}

	virtual FDynamicValue GetValue_Implementation(const FString& ValueName) override{ return DataReceived; }


	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, SaveGame)
	ARadioTransmitter* TransmitterReference;

	UPROPERTY(Replicated, VisibleInstanceOnly)
	FDynamicValue DataReceived;
};
