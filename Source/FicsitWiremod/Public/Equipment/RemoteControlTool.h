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
};

UCLASS()
class FICSITWIREMOD_API ARemoteControlTool : public AWiremodBaseTool
{
	GENERATED_BODY()

public:

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

	UFUNCTION(BlueprintCallable)
	void SetText(FString Text){ Data.TextInput = Text; HandleDataUpdate(); }

	UFUNCTION(BlueprintCallable)
	void HandleDataUpdate() const
	{
		auto RCO = Cast<UWiremodRemoteCalls>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(UWiremodRemoteCalls::StaticClass()));
		RCO->UpdateRemoteControlData(Receiver, Data, UWiremodBlueprintUtils::GetSetterObject());
	}

	UFUNCTION(BlueprintCallable)
	void OnReceiverChanged(ARemoteControlReceiver* NewReceiver)
	{
		auto RCO = Cast<UWiremodRemoteCalls>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(UWiremodRemoteCalls::StaticClass()));
		RCO->SetBuildableOwner(Receiver, nullptr, UWiremodBlueprintUtils::GetSetterObject());

		Receiver = NewReceiver;
		if(Receiver) RCO->SetBuildableOwner(Receiver, UWiremodBlueprintUtils::GetSetterObject(), UWiremodBlueprintUtils::GetSetterObject());
		HandleDataUpdate();
	}
	
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	FRemoteControlData Data;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	ARemoteControlReceiver* Receiver;
};
