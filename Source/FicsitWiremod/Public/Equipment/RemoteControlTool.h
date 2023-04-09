// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodBaseTool.h"
#include "Behaviour/WiremodRemoteCalls.h"
#include "Behaviour/Communications/RemoteControlReceiver.h"
#include "RemoteControlTool.generated.h"

UCLASS()
class FICSITWIREMOD_API ARemoteControlTool : public AWiremodBaseTool
{
	GENERATED_BODY()

public:

	virtual void WasEquipped_Implementation() override
	{
		if(IsLocallyControlled())
		{
			DisplayHudHints();
			//Press
			InputComponent->BindAction("PrimaryFire", IE_Pressed, this, &ARemoteControlTool::HandleKey1_Pressed);
			InputComponent->BindAction("SecondaryFire", IE_Pressed, this, &ARemoteControlTool::HandleKey2_Pressed);
			InputComponent->BindAction("FicsitWiremod.RemoteControlKey3", IE_Pressed, this, &ARemoteControlTool::HandleKey3_Pressed);
			InputComponent->BindAction("FicsitWiremod.RemoteControlKey4", IE_Pressed, this, &ARemoteControlTool::HandleKey4_Pressed);
			InputComponent->BindAction("FicsitWiremod.RemoteControlKey5", IE_Pressed, this, &ARemoteControlTool::HandleKey5_Pressed);

			//Release
			InputComponent->BindAction("PrimaryFire", IE_Released, this, &ARemoteControlTool::HandleKey1_Released);
			InputComponent->BindAction("SecondaryFire", IE_Released, this, &ARemoteControlTool::HandleKey2_Released);
			InputComponent->BindAction("FicsitWiremod.RemoteControlKey3", IE_Released, this, &ARemoteControlTool::HandleKey3_Released);
			InputComponent->BindAction("FicsitWiremod.RemoteControlKey4", IE_Released, this, &ARemoteControlTool::HandleKey4_Released);
			InputComponent->BindAction("FicsitWiremod.RemoteControlKey5", IE_Released, this, &ARemoteControlTool::HandleKey5_Released);
		}
	}
	virtual void WasUnEquipped_Implementation() override { if(IsLocallyControlled()) HideHudHints(); }
	
	void HandleKey1_Pressed() { Data.Keys[0] = true; HandleDataUpdate(); }
	void HandleKey2_Pressed() { Data.Keys[1] = true; HandleDataUpdate(); }
	void HandleKey3_Pressed() { Data.Keys[2] = true; HandleDataUpdate(); }
	void HandleKey4_Pressed() { Data.Keys[3] = true; HandleDataUpdate(); }
	void HandleKey5_Pressed() { Data.Keys[4] = true; HandleDataUpdate(); }

	void HandleKey1_Released() { Data.Keys[0] = false; HandleDataUpdate(); }
	void HandleKey2_Released() { Data.Keys[1] = false; HandleDataUpdate(); }
	void HandleKey3_Released() { Data.Keys[2] = false; HandleDataUpdate(); }
	void HandleKey4_Released() { Data.Keys[3] = false; HandleDataUpdate(); }
	void HandleKey5_Released() { Data.Keys[4] = false; HandleDataUpdate(); }

	UFUNCTION(BlueprintCallable)
	void SetText(FString Text){ Data.TextInput = Text; HandleDataUpdate(); }

	UFUNCTION(BlueprintCallable)
	void HandleDataUpdate() const
	{
		auto RCO = Cast<UWiremodRemoteCalls>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(UWiremodRemoteCalls::StaticClass()));
		RCO->UpdateRemoteControlData_Implementation(Receiver, Data, UWiremodBlueprintUtils::GetSetterObject());
	}

	UFUNCTION(BlueprintCallable)
	void OnReceiverChanged(ARemoteControlReceiver* NewReceiver)
	{
		auto RCO = Cast<UWiremodRemoteCalls>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(UWiremodRemoteCalls::StaticClass()));
		RCO->SetBuildableOwner_Implementation(Receiver, nullptr, UWiremodBlueprintUtils::GetSetterObject());

		Receiver = NewReceiver;
		if(Receiver) RCO->SetBuildableOwner_Implementation(Receiver, UWiremodBlueprintUtils::GetSetterObject(), UWiremodBlueprintUtils::GetSetterObject());
		HandleDataUpdate();
	}
	
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	FRemoteControlData Data;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	ARemoteControlReceiver* Receiver;
};
