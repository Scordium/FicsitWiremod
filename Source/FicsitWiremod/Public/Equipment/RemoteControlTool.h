// 

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FGPlayerController.h"
#include "WiremodBaseTool.h"
#include "Behaviour/WiremodRemoteCalls.h"
#include "Behaviour/Communications/RemoteControlReceiver.h"
#include "Utility/CircuitryInputMappings.h"
#include "Utility/WiremodBlueprintUtils.h"
#include "RemoteControlTool.generated.h"


UCLASS()
class URemoteControlInputsContext : public UFGInputMappingContext
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UInputAction* RemoteControlKey1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UInputAction* RemoteControlKey2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UInputAction* RemoteControlKey3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UInputAction* RemoteControlKey4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UInputAction* RemoteControlKey5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UInputAction* RemoteControlKey6;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UInputAction* RemoteControlKey7;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UInputAction* RemoteControlKey8;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UInputAction* RemoteControlKey9;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UInputAction* RemoteControlKey10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UInputAction* DecreaseSet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UInputAction* IncreaseSet;
};

UCLASS()
class FICSITWIREMOD_API ARemoteControlTool : public AWiremodBaseTool
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaSeconds) override
	{
		bool SuccessfulHit;
		FVector Location;
		AActor* HitActor = GetTargetLookAt(10000000, ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);

		if(HitActor != Data.HitActor)
		{
			Data.HitActor = HitActor;
			HandleDataUpdate();
		}
	}

	virtual void WasEquipped_Implementation() override
	{
		if(IsLocallyControlled())
		{
			Super::WasEquipped_Implementation();
			DisplayHudHints();
			
			auto EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
			auto InputsContext = GetFirstContextOfType<URemoteControlInputsContext>();
			EnhancedInput->BindAction(InputsContext->RemoteControlKey1, ETriggerEvent::Triggered, this, &ARemoteControlTool::HandleKey1);
			EnhancedInput->BindAction(InputsContext->RemoteControlKey2, ETriggerEvent::Triggered, this, &ARemoteControlTool::HandleKey2);
			EnhancedInput->BindAction(InputsContext->RemoteControlKey3, ETriggerEvent::Triggered, this, &ARemoteControlTool::HandleKey3);
			EnhancedInput->BindAction(InputsContext->RemoteControlKey4, ETriggerEvent::Triggered, this, &ARemoteControlTool::HandleKey4);
			EnhancedInput->BindAction(InputsContext->RemoteControlKey5, ETriggerEvent::Triggered, this, &ARemoteControlTool::HandleKey5);
			EnhancedInput->BindAction(InputsContext->RemoteControlKey6, ETriggerEvent::Triggered, this, &ARemoteControlTool::HandleKey6);
			EnhancedInput->BindAction(InputsContext->RemoteControlKey7, ETriggerEvent::Triggered, this, &ARemoteControlTool::HandleKey7);
			EnhancedInput->BindAction(InputsContext->RemoteControlKey8, ETriggerEvent::Triggered, this, &ARemoteControlTool::HandleKey8);
			EnhancedInput->BindAction(InputsContext->RemoteControlKey9, ETriggerEvent::Triggered, this, &ARemoteControlTool::HandleKey9);
			EnhancedInput->BindAction(InputsContext->RemoteControlKey10, ETriggerEvent::Triggered, this, &ARemoteControlTool::HandleKey10);
			EnhancedInput->BindAction(InputsContext->DecreaseSet, ETriggerEvent::Triggered, this, &ARemoteControlTool::DecreaseSet);
			EnhancedInput->BindAction(InputsContext->IncreaseSet, ETriggerEvent::Triggered, this, &ARemoteControlTool::IncreaseSet);
		}
	}
	virtual void WasUnEquipped_Implementation() override
	{
		if(IsLocallyControlled())
		{
			Super::WasUnEquipped_Implementation();
			HideHudHints();
		}
	}
	
	void HandleKey1(const FInputActionValue& Value) { Data.Keys[0] = Value.Get<bool>(); HandleDataUpdate(); }
	void HandleKey2(const FInputActionValue& Value) { Data.Keys[1] = Value.Get<bool>(); HandleDataUpdate(); }
	void HandleKey3(const FInputActionValue& Value) { Data.Keys[2] = Value.Get<bool>(); HandleDataUpdate(); }
	void HandleKey4(const FInputActionValue& Value) { Data.Keys[3] = Value.Get<bool>(); HandleDataUpdate(); }
	void HandleKey5(const FInputActionValue& Value) { Data.Keys[4] = Value.Get<bool>(); HandleDataUpdate(); }
	void HandleKey6(const FInputActionValue& Value) { Data.Keys[5] = Value.Get<bool>(); HandleDataUpdate(); }
	void HandleKey7(const FInputActionValue& Value) { Data.Keys[6] = Value.Get<bool>(); HandleDataUpdate(); }
	void HandleKey8(const FInputActionValue& Value) { Data.Keys[7] = Value.Get<bool>(); HandleDataUpdate(); }
	void HandleKey9(const FInputActionValue& Value) { Data.Keys[8] = Value.Get<bool>(); HandleDataUpdate(); }
	void HandleKey10(const FInputActionValue& Value) { Data.Keys[9] = Value.Get<bool>(); HandleDataUpdate(); }
	void DecreaseSet(const FInputActionValue& Value)
	{
		if(Value.Get<bool>())
		{
			Data.CurrentSet--;
			HandleDataUpdate();
		}
	}
	void IncreaseSet(const FInputActionValue& Value)
	{
		if(Value.Get<bool>())
		{
			Data.CurrentSet++;
			HandleDataUpdate();
		}
	}

	UFUNCTION(BlueprintCallable)
	void SetText(FString Text){ Data.TextInput = Text; HandleDataUpdate(); }

	UFUNCTION(BlueprintCallable)
	void HandleDataUpdate() const
	{
		auto RCO = Cast<UWiremodRemoteCalls>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(UWiremodRemoteCalls::StaticClass()));
		for(auto Receiver : Receivers) RCO->UpdateRemoteControlData(Receiver, Data, UWiremodBlueprintUtils::GetSetterObject());
	}

	UFUNCTION(BlueprintCallable)
	void AddReceiver(ARemoteControlReceiver* Receiver)
	{
		auto RCO = Cast<UWiremodRemoteCalls>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(UWiremodRemoteCalls::StaticClass()));
		RCO->SetBuildableOwner(Receiver, UWiremodBlueprintUtils::GetSetterObject(), UWiremodBlueprintUtils::GetSetterObject());
		RCO->UpdateRemoteControlData(Receiver, FRemoteControlData(), UWiremodBlueprintUtils::GetSetterObject());
		Receivers.AddUnique(Receiver);
	}

	UFUNCTION(BlueprintCallable)
	void RemoveReceiver(ARemoteControlReceiver* Receiver)
	{
		auto RCO = Cast<UWiremodRemoteCalls>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(UWiremodRemoteCalls::StaticClass()));
		RCO->UpdateRemoteControlData(Receiver, FRemoteControlData(), UWiremodBlueprintUtils::GetSetterObject());
		RCO->SetBuildableOwner(Receiver, nullptr, UWiremodBlueprintUtils::GetSetterObject());
		Receivers.Remove(Receiver);
	}
	
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	FRemoteControlData Data;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	TArray<ARemoteControlReceiver*> Receivers;
};
