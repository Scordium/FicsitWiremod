// 

#pragma once

#include "CoreMinimal.h"
#include "FGRemoteCallObject.h"
#include "CommonLib/BackwardsCompatibilityHandler.h"
#include "Communications/RemoteControlReceiver.h"
#include "Displays/ManagedSign/ManagedSign.h"
#include "Miscellaneous/CustomStructProcessor.h"
#include "WiremodRemoteCalls.generated.h"


USTRUCT(BlueprintType)
struct FCustomStructReplicatable
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Name;

	UPROPERTY(BlueprintReadWrite)
	TArray<FNamedValue> Values;


	FCustomStruct Convert(UObject* WorldContext) const
	{
		//This is used to replicate custom struct data from clients to server because the server doesn't know about client-sided uobjects which dynamic values derive from.
		FCustomStruct Value;
		Value.Name = Name;
	
		for(auto& Val : Values)
		{
			auto Field = FNamedDynamicValue(Val.Name, UCCDynamicValueUtils::FromWrapper(WorldContext, Val.Value));
			Value.Values.Add(Field);
		}

		return Value;
	}
	
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class FICSITWIREMOD_API UWiremodRemoteCalls : public UFGRemoteCallObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void ConnectWiremodObject(UObject* Buildable, const FConnectionData& Data, int Index, UObject* Setter);
	
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void ConnectNonWiremodObject(const FDynamicConnectionData& Connection, int Index, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void ResetConnections(UObject* Buildable, int Index, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void UpdateRemoteControlData(ARemoteControlReceiver* Receiver, const FRemoteControlData& NewData, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetBuildableOwner(AFGBuildable* Buildable, UObject* NewOwner, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetCustomStructData(ACustomStructProcessor* Processor, const FCustomStructReplicatable& Data, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetSignLayout(AManagedSign* Sign, const FManagedSignData& Data, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void CreateGlobalConnection(AFGBuildable* Buildable, int Index, const FDynamicValueStringWrapper& Value, const FConnectionData& Input, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetConfiguringRule(AFGBuildable* Buildable, EWiremodInteractionRule Rule, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetResettingRule(AFGBuildable* Buildable, EWiremodInteractionRule Rule, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetConnectingRule(AFGBuildable* Buildable, EWiremodInteractionRule Rule, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetCanDismantle(AFGBuildable* Buildable, bool CanDismantle, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetWireColor(AFGBuildable* Buildable, int Index, FLinearColor Color, UObject* Setter);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void SetWireHidden(AFGBuildable* Buildable, int Index, bool Hidden, UObject* Setter);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(UWiremodRemoteCalls, RandomPropertyBecauseDocsSaySo)
	}

	UPROPERTY(Replicated)
	int RandomPropertyBecauseDocsSaySo;
};
