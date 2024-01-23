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

void UWiremodRemoteCalls::CreateGlobalConnection_Implementation(AFGBuildable* Buildable, int Index, const FDynamicValueStringWrapper& Value, const FConnectionData& Input, UObject* Setter)
{
	auto Data = FConnectionData(UCCDynamicValueUtils::FromWrapper(this, Value), FName("Value"), Value.Type);
	Data.WireHidden = true;

	if(Buildable->GetClass()->ImplementsInterface(ICircuitryProcessableInterface::UClassType::StaticClass()))
		ICircuitryProcessableInterface::Execute_OnInputConnected(Buildable, Data, Index, Setter);
	else
	{
		auto Dynamic = FDynamicConnectionData(Data, Input);
		AWiremodVanillaConnections::Self->AddConnection(Dynamic, Index, Setter);
	}
}

void UWiremodRemoteCalls::SetConfiguringRule_Implementation(AFGBuildable* Buildable, EWiremodInteractionRule Rule, UObject* Setter)
{
	if(auto Circuitry = Cast<AFGWiremodBuildable>(Buildable)) Circuitry->SetConfiguringRule(Rule, Setter);
	else AWiremodVanillaConnections::Self->SetConfiguringRule(Buildable, Rule, Setter);
}

void UWiremodRemoteCalls::SetConnectingRule_Implementation(AFGBuildable* Buildable, EWiremodInteractionRule Rule, UObject* Setter)
{
	if(auto Circuitry = Cast<AFGWiremodBuildable>(Buildable)) Circuitry->SetConnectingRule(Rule, Setter);
	else AWiremodVanillaConnections::Self->SetConnectingRule(Buildable, Rule, Setter);
}

void UWiremodRemoteCalls::SetResettingRule_Implementation(AFGBuildable* Buildable, EWiremodInteractionRule Rule, UObject* Setter)
{
	if(auto Circuitry = Cast<AFGWiremodBuildable>(Buildable)) Circuitry->SetDisconnectingRule(Rule, Setter);
	else AWiremodVanillaConnections::Self->SetDisconnectingRule(Buildable, Rule, Setter);
}

void UWiremodRemoteCalls::SetCanDismantle_Implementation(AFGBuildable* Buildable, bool CanDismantle, UObject* Setter)
{
	if(auto Circuitry = Cast<AFGWiremodBuildable>(Buildable)) Circuitry->SetCanDismantle(CanDismantle, Setter);
	else AWiremodVanillaConnections::Self->SetCanDismantle(Buildable, CanDismantle, Setter);
}

void UWiremodRemoteCalls::SetWireColor_Implementation(AFGBuildable* Buildable, int Index, FLinearColor Color, UObject* Setter)
{
	if(auto Circuitry = Cast<AFGWiremodBuildable>(Buildable)) Circuitry->SetWireColor(Index, Color, Setter);
	else AWiremodVanillaConnections::Self->SetWireColor(Buildable, Index, Color, Setter);
}

void UWiremodRemoteCalls::SetWireHidden_Implementation(AFGBuildable* Buildable, int Index, bool Hidden, UObject* Setter)
{
	if(auto Circuitry = Cast<AFGWiremodBuildable>(Buildable)) Circuitry->SetWireHidden(Index, Hidden, Setter);
	else AWiremodVanillaConnections::Self->SetWireHidden(Buildable, Index, Hidden, Setter);
}
