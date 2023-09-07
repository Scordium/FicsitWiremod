// 


#include "Behaviour/WiremodRemoteCalls.h"

#include "Utility/WiremodBlueprintUtils.h"

void UWiremodRemoteCalls::ConnectWiremodObject_Implementation(UObject* Buildable, const FConnectionData& Data, int Index, UObject* Setter)
{
	if(!UWiremodBlueprintUtils::IsCircuitry(Buildable)) return;
	ICircuitryProcessableInterface::Execute_OnInputConnected(Buildable, Data, Index, Setter);
}

bool UWiremodRemoteCalls::ConnectWiremodObject_Validate(UObject* Buildable, const FConnectionData& Data, int Index, UObject* Setter){ return true; }

void UWiremodRemoteCalls::ConnectNonWiremodObject_Implementation(const FDynamicConnectionData& Connection, int Index, UObject* Setter)
{
	AWiremodVanillaConnections::Self->AddConnection(Connection, Index, Setter);
}

bool UWiremodRemoteCalls::ConnectNonWiremodObject_Validate(const FDynamicConnectionData& Connection, int Index, UObject* Setter){ return true; }

void UWiremodRemoteCalls::ResetConnections_Implementation(UObject* Buildable, int Index, UObject* Setter)
{
	if(UWiremodBlueprintUtils::IsCircuitry(Buildable)) ICircuitryProcessableInterface::Execute_OnInputDisconnected(Buildable, Index, Setter);
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

void UWiremodRemoteCalls::SetCustomStructData_Implementation(ACustomStructProcessor* Processor, const FCustomStructReplicatable& Data, UObject* Setter)
{
	Processor->SetCustomStruct(Data.Convert(Processor), Setter);
}

void UWiremodRemoteCalls::SetSignLayout_Implementation(AManagedSign* Sign, const FManagedSignData& Data, UObject* Setter)
{
	if(Sign) Sign->ApplySignLayout(Data, Setter);
}
