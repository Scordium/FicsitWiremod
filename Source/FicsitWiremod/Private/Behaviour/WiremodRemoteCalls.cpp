// 


#include "Behaviour/WiremodRemoteCalls.h"

void UWiremodRemoteCalls::ConnectWiremodObject_Implementation(UObject* Buildable, const FNewConnectionData& Data, int Index, UObject* Setter)
{
	if(!UWiremodBlueprintUtils::IsWiremod(Buildable)) return;
	IWiremodInterface::Execute_OnInputConnected(Buildable, Data, Index, Setter);
}

bool UWiremodRemoteCalls::ConnectWiremodObject_Validate(UObject* Buildable, const FNewConnectionData& Data, int Index, UObject* Setter){ return true; }

void UWiremodRemoteCalls::ConnectNonWiremodObject_Implementation(const FDynamicConnectionData& Connection, int Index, UObject* Setter)
{
	AWiremodVanillaConnections::Self->AddConnection(Connection, Index, Setter);
}

bool UWiremodRemoteCalls::ConnectNonWiremodObject_Validate(const FDynamicConnectionData& Connection, int Index, UObject* Setter){ return true; }

void UWiremodRemoteCalls::ResetConnections_Implementation(UObject* Buildable, int Index, UObject* Setter)
{
	if(UWiremodBlueprintUtils::IsWiremod(Buildable)) IWiremodInterface::Execute_OnInputDisconnected(Buildable, Index, Setter);
	else AWiremodVanillaConnections::Self->ResetConnection(Buildable, Index, Setter);
}

bool UWiremodRemoteCalls::ResetConnections_Validate(UObject* Buildable, int Index, UObject* Setter){ return true; }

void UWiremodRemoteCalls::UpdateRemoteControlData_Implementation(ARemoteControlReceiver* Receiver, const FRemoteControlData& NewData, UObject* Setter)
{
	if(!Receiver) return;

	Receiver->SetData(NewData, Setter);
}

bool UWiremodRemoteCalls::UpdateRemoteControlData_Validate(ARemoteControlReceiver* Receiver, const FRemoteControlData& NewData, UObject* Setter)
{
	return true;
}

void UWiremodRemoteCalls::SetBuildableOwner_Implementation(AFGBuildable* Buildable, UObject* NewOwner, UObject* Setter)
{
	if(auto WiremodBuildable = Cast<AFGWiremodBuildable>(Buildable)) WiremodBuildable->SetBuildableOwner(NewOwner, Setter);
	else AWiremodVanillaConnections::Self->SetBuildableOwner(Buildable, NewOwner, Setter);
}

bool UWiremodRemoteCalls::SetBuildableOwner_Validate(AFGBuildable* Buildable, UObject* NewOwner, UObject* Setter) { return true; }
