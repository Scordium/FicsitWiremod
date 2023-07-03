// 

#pragma once

#include "CoreMinimal.h"
#include "RadioTransmitter.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/PlayerOwnedClipboardData.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "RadioReceiver.generated.h"


UCLASS()
class URadioReceiverClipboard : public UPlayerOwnedClipboardData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	ARadioTransmitter* Transmitter;
};


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
		bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
		
		WroteSomething |= Channel->ReplicateSubobject(DataReceived, *Bunch, *RepFlags);

		return WroteSomething;
	}

	virtual bool CanUseFactoryClipboard_Implementation() override { return true; }
	virtual TSubclassOf<UFGFactoryClipboardSettings> GetClipboardSettingsClass_Implementation() override { return URadioReceiverClipboard::StaticClass(); }
	virtual TSubclassOf<UObject> GetClipboardMappingClass_Implementation() override { return StaticClass(); }
	virtual UFGFactoryClipboardSettings* CopySettings_Implementation() override
	{
		auto Settings = NewObject<URadioReceiverClipboard>(this);
		Settings->Transmitter = TransmitterReference;

		return Settings;
	}

	virtual bool PasteSettings_Implementation(UFGFactoryClipboardSettings* factoryClipboard) override
	{
		if(auto Clipboard = Cast<URadioReceiverClipboard>(factoryClipboard))
		{
			TransmitterReference = Clipboard->Transmitter;
			return true;
		}

		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetTransmitter(ARadioTransmitter* NewTransmitter, UObject* Actor)
	{
		if(!GetCanConfigure(Actor)) return;

		TransmitterReference = NewTransmitter;
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override{ return DataReceived; }


	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, SaveGame)
	ARadioTransmitter* TransmitterReference;

	UPROPERTY(Replicated, VisibleInstanceOnly)
	UCCDynamicValueBase* DataReceived;
};
