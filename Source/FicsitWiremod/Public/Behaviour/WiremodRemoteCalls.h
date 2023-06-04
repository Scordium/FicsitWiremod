// 

#pragma once

#include "CoreMinimal.h"
#include "FGRemoteCallObject.h"
#include "WiremodReflection.h"
#include "Communications/RemoteControlReceiver.h"
#include "Miscellaneous/CustomStructProcessor.h"
#include "Utility/WiremodBlueprintUtils.h"
#include "WiremodRemoteCalls.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class FICSITWIREMOD_API UWiremodRemoteCalls : public UFGRemoteCallObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Reliable, WithValidation, Server)
	void ConnectWiremodObject(UObject* Buildable, const FNewConnectionData& Data, int Index, UObject* Setter);
	
	UFUNCTION(BlueprintCallable, Reliable, WithValidation, Server)
	void ConnectNonWiremodObject(const FDynamicConnectionData& Connection, int Index, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, WithValidation, Server)
	void ResetConnections(UObject* Buildable, int Index, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, WithValidation, Server)
	void UpdateRemoteControlData(ARemoteControlReceiver* Receiver, const FRemoteControlData& NewData, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, WithValidation, Server)
	void SetBuildableOwner(AFGBuildable* Buildable, UObject* NewOwner, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetCustomStructData(ACustomStructProcessor* Processor, const FCustomStruct& Data, UObject* Setter);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(UWiremodRemoteCalls, RandomPropertyBecauseDocsSaySo)
	}

	UPROPERTY(Replicated)
	int RandomPropertyBecauseDocsSaySo;
};
