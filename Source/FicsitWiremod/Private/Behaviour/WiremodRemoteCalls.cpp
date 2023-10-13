// 


#include "Behaviour/WiremodRemoteCalls.h"

#include "Utility/WiremodBlueprintUtils.h"

void UWiremodRemoteCalls::ConnectWiremodObject_Implementation(UObject* Buildable, const FConnectionData& Data, int Index, UObject* Setter)
{
	if(!UWiremodBlueprintUtils::IsCircuitry(Buildable)) return;
	ICircuitryProcessableInterface::Execute_OnInputConnected(Buildable, Data, Index, Setter);
}

void UWiremodRemoteCalls::ConnectNonWiremodObject_Implementation(const FDynamicConnectionData& Connection, int Index, UObject* Setter)
{
	AWiremodVanillaConnections::Self->AddConnection(Connection, Index, Setter);
}

void UWiremodRemoteCalls::ResetConnections_Implementation(UObject* Buildable, int Index, UObject* Setter)
{
	if(UWiremodBlueprintUtils::IsCircuitry(Buildable)) ICircuitryProcessableInterface::Execute_OnInputDisconnected(Buildable, Index, Setter);
	else AWiremodVanillaConnections::Self->ResetConnection(Buildable, Index, Setter);
}

void UWiremodRemoteCalls::UpdateRemoteControlData_Implementation(ARemoteControlReceiver* Receiver, const FRemoteControlData& NewData, UObject* Setter)
{
	if(Receiver) Receiver->SetData(NewData, Setter);
}

void UWiremodRemoteCalls::SetBuildableOwner_Implementation(AFGBuildable* Buildable, UObject* NewOwner, UObject* Setter)
{
	if(auto WiremodBuildable = Cast<AFGWiremodBuildable>(Buildable)) WiremodBuildable->SetBuildableOwner(NewOwner, Setter);
	else AWiremodVanillaConnections::Self->SetBuildableOwner(Buildable, NewOwner, Setter);
}

void UWiremodRemoteCalls::SetCustomStructData_Implementation(ACustomStructProcessor* Processor, const FCustomStructReplicatable& Data, UObject* Setter)
{
	if(Processor) Processor->SetCustomStruct(Data.Convert(Processor), Setter);
}

void UWiremodRemoteCalls::SetSignLayout_Implementation(AManagedSign* Sign, const FManagedSignData& Data, UObject* Setter)
{
	if(Sign) Sign->ApplySignLayout(Data, Setter);
}
