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
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if(TransmitterReference)
		{
			DataReceived = UCCDynamicValueUtils::FromValue(TransmitterReference->GetConnection(0), DataReceived);
			SetOutputType(0, DataReceived ? DataReceived->ConnectionType.GetValue() : DefaultConnectionType.GetValue());
		}
		else SetOutputType(0, DefaultConnectionType.GetValue());
	}

	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ARadioReceiver, TransmitterReference);
		DOREPLIFETIME(ARadioReceiver, DataReceived);
	}

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& OutObjects) override
	{
		Super::GatherReplicatedObjects_Implementation(OutObjects);
		OutObjects.Add(DataReceived);
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
		PERMISSION_CHECK(Actor);

		TransmitterReference = NewTransmitter;
	}

	UFUNCTION(BlueprintCallable)
	void SetDefaultConnectionType(EConnectionType Type, UObject* Setter)
	{
		PERMISSION_CHECK(Setter);

		DefaultConnectionType = Type;
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override{ return DataReceived; }

	UFUNCTION(BlueprintCallable)
	TArray<ARadioTransmitter*> SortTransmitters(TArray<ARadioTransmitter*> Input)
	{
		const auto Sort = [&](ARadioTransmitter& Lhs, ARadioTransmitter& Rhs)-> bool
		{
			const double DistanceLeft = FVector::Distance(GetActorLocation(), Lhs.GetActorLocation());
			const double DistanceRight = FVector::Distance(GetActorLocation(), Rhs.GetActorLocation());
			return DistanceLeft < DistanceRight;
		};
		
		Input.StableSort(Sort);

		const auto ReceiverName = OwnerData.CustomName.ToString();
		if(ReceiverName.Len() > 0)
		{
			//Put transmitters that match the receiver's name at the top of the list
			TArray SortNameMatches(Input);
			for(int i = SortNameMatches.Num() - 1; i >= 0; i--)
			{
				const auto Transmitter = SortNameMatches[i];
				const auto Name = Transmitter->OwnerData.CustomName.ToString();
				if(Name.Contains(ReceiverName))
				{
					Input.Remove(Transmitter);
					Input.Insert(Transmitter, 0);
				}
			}
		}

		return Input;
	}

	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, SaveGame)
	ARadioTransmitter* TransmitterReference;

	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	UCCDynamicValueBase* DataReceived;

	UPROPERTY(Replicated, SaveGame, BlueprintReadOnly, VisibleInstanceOnly)
	TEnumAsByte<EConnectionType> DefaultConnectionType = Unknown;
};