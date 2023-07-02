// 
#include "Behaviour/FGWiremodBuildable.h"

#include "WiremodReflection.h"
#include "Buildables/FGBuildableBlueprintDesigner.h"

void AFGWiremodBuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsActiveEntity()) Process(DeltaTime);
}

FConnectionData AFGWiremodBuildable::GetConnection(int Index)
{
	if(!InputConnections.IsValidIndex(Index)) return FConnectionData(nullptr, FName());
	return InputConnections[Index];
}

void AFGWiremodBuildable::GetAllConnected(TArray<FConnectionData>& Out)
{
	for (auto InputConnection : InputConnections)
		if(InputConnection.IsValid())
			Out.Add(InputConnection);
}

bool AFGWiremodBuildable::IsConnected(int Index)
{
	if(!InputConnections.IsValidIndex(Index)) return false;
	return InputConnections[Index].IsValid();
}

int AFGWiremodBuildable::GetNumConnected()
{
	int i = 0;
	for (auto Connection : InputConnections)
		if(IsValid(Connection.Object)) i++;
		
	return i;
}

void AFGWiremodBuildable::SetOutputType(int Index, EConnectionType Type)
{
	if(!ConnectionsInfo.Outputs.IsValidIndex(Index)) return;
	ConnectionsInfo.Outputs[Index].ConnectionType = Type;
}

void AFGWiremodBuildable::SetInputType(int Index, EConnectionType Type)
{
	if(!ConnectionsInfo.Inputs.IsValidIndex(Index)) return;
	ConnectionsInfo.Inputs[Index].ConnectionType = Type;
}

EConnectionType AFGWiremodBuildable::GetOutputType(int Index)
{
	if(!ConnectionsInfo.Outputs.IsValidIndex(Index)) return Unknown;
	return ConnectionsInfo.Outputs[Index].ConnectionType;
}

EConnectionType AFGWiremodBuildable::GetInputType(int Index)
{
	if(!ConnectionsInfo.Inputs.IsValidIndex(Index)) return Unknown;
	return ConnectionsInfo.Inputs[Index].ConnectionType;
}

void AFGWiremodBuildable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFGWiremodBuildable, ConnectionsInfo);
	DOREPLIFETIME(AFGWiremodBuildable, InputConnections);
	DOREPLIFETIME(AFGWiremodBuildable, OwnerData);
}

void AFGWiremodBuildable::OnInputConnected_Implementation(const FConnectionData& Data, int Index, UObject* Setter)
{
	if(!GetCanConnect(Setter)) return;
	OnInputConnected_Internal(Data, Index);
}

void AFGWiremodBuildable::OnInputConnected_Internal(const FConnectionData& Data, int Index)
{
	if(InputConnections.Num() <= Index)
		InputConnections.SetNum(Index + 1);

	auto ConvertedPositions = ConvertConnections(Data.WirePositions);

	//A small hack to force redraw the wire if it doesn't have a complex shape. Implemented for usage with mods that can move buildables (i.e. MicroManage)
	if(ConvertedPositions.Num() < 3)
	{
		//Check if the starting point is the object's location.
		//If it is then it's safe to make adaptive wire. If not - don't do anything. The only thing that will change is that wire won't redraw after the objects were moved around.
		//There's no need to bother making a wire at all if the object is not an actor, so check for that as well.
		if(auto actor = Cast<AActor>(Data.Object); actor && Data.WirePositions.Num() && Data.WirePositions[0] == actor->GetActorLocation()) ConvertedPositions.Empty();
	}
	InputConnections[Index] = FConnectionData(Data, ConvertedPositions);
	
	DrawWires();
}





void AFGWiremodBuildable::OnInputDisconnected_Implementation(int Index, UObject* Setter)
{
	if(!GetCanDisconnect(Setter)) return;
	OnInputDisconnected_Internal(Index);
}

void AFGWiremodBuildable::OnInputDisconnected_Internal(int Index)
{
	if(Index == -1)InputConnections.Empty();
	else
	{
		if(InputConnections.IsValidIndex(Index))
			InputConnections[Index] = FConnectionData();
	}
	
	DrawWires();
}


TArray<FBuildingConnection> AFGWiremodBuildable::GetConnectionsInfo_Implementation(EConnectionDirection direction, int& Count, FBuildableNote& Note)
{
	Note = ConnectionsInfo.Note;
	
	switch (direction)
	{
	case Input:
		{
			Count = ConnectionsInfo.Inputs.Num();
			return ConnectionsInfo.Inputs;
		}
	case Output:
		{
			Count = ConnectionsInfo.Outputs.Num();
			return ConnectionsInfo.Outputs;
		}
		//Possibly add more cases here for other behaviors?
	default: return TArray<FBuildingConnection>();
	}
}

TSubclassOf<UUserWidget> AFGWiremodBuildable::GetCompactWidget_Implementation()
{
	return CompactWidget;
}


int AFGWiremodBuildable::netFunc_getFunctionReturnType(FString FunctionName)
{
	for(auto conn : GetConnections_Implementation(Input))
	{
		if(conn.FunctionName.ToString() == FunctionName || conn.DisplayName == FunctionName)
			return conn.ConnectionType.GetValue();
	}

	return -1;
}
bool AFGWiremodBuildable::netFunc_getWireBool(FString FunctionName, bool DefaultValue){ return FConnectionData(this, FName(FunctionName), Boolean).GetBool(DefaultValue); }
float AFGWiremodBuildable::netFunc_getWireNumber(FString FunctionName, float DefaultValue) { return FConnectionData(this, FName(FunctionName), Number).GetFloat(DefaultValue); }
FString AFGWiremodBuildable::netFunc_getWireString(FString FunctionName, FString DefaultValue) { return FConnectionData(this, FName(FunctionName), String).GetString(DefaultValue); }
FVector AFGWiremodBuildable::netFunc_getWireVector(FString FunctionName, FVector DefaultValue) { return FConnectionData(this, FName(FunctionName), Vector).GetVector(DefaultValue); }
FLinearColor AFGWiremodBuildable::netFunc_getWireColor(FString FunctionName, FLinearColor DefaultValue) { return FConnectionData(this, FName(FunctionName), Color).GetColor(DefaultValue); }
FString AFGWiremodBuildable::netFunc_getDebuggerOutputString(FString FunctionName) { return FConnectionData(this, FName(FunctionName), (EConnectionType) netFunc_getFunctionReturnType(FunctionName)).GetStringifiedValue(); }
bool AFGWiremodBuildable::netFunc_isBlueprinted()
{
	return IsValid(mBlueprintDesigner);
}

void AFGWiremodBuildable::GetInputOccupationStatus(EConnectionType AllowedType, TArray<TEnumAsByte<EConnectionOccupationState>>& Out)
{
	auto Inputs = GetConnections_Implementation(Input);

	for(int i = 0; i < Inputs.Num(); i++)
	{
		auto Connection = GetConnection(i);
		if(!Connection.IsValid())
		{
			bool ValidPair = UConnectionTypeFunctions::IsValidConnectionPair(Inputs[i].ConnectionType, AllowedType);
			Out.Add(ValidPair ? Free : Disabled);
		}
		else
		{
			if(Connection.Object->GetClass()->IsChildOf(UCCDynamicValueBase::StaticClass())) Out.Add(GlobalConnection);
			else Out.Add(Connected);
		}
	}
}

void AFGWiremodBuildable::netFunc_getAllWiremodFuncs(TArray<FString>& Out)
{
	for(auto conn : GetConnections_Implementation(Output))
	{
		Out.Add(conn.FunctionName.ToString());
	}
}

void AFGWiremodBuildable::DrawWires_Implementation()
{
	if(!HasAuthority()) return;
	
	//First remove all existing wires
	TArray<UActorComponent*> ChildComponents;
	GetComponents(UChildActorComponent::StaticClass(), ChildComponents);
	for (auto ChildComponent : ChildComponents)
	{
		auto ChildActorComponent = Cast<UChildActorComponent>(ChildComponent);
		if(auto Wire = Cast<AConnectionWireBase>(ChildActorComponent->GetChildActor()))
		{
			Wire->Invalidate();
			ChildActorComponent->DestroyChildActor();
			ChildActorComponent->DestroyComponent();
		}
	}

	//Get all inputs and if there are none, return.
	auto InputInfoList = GetConnections_Implementation(Input);
	if(InputInfoList.Num() == 0) return;

	//Visible wire class to use as wire
	auto WireClass = AConnectionWireBase::StaticClass();
	
	//Create new wires
	for (int i = 0; i < InputConnections.Num(); i++)
	{
		FConnectionData ConnectionData = GetConnection(i);
		if(!ConnectionData.IsValidForWire()) continue;
		if(ConnectionData.WireHidden) continue;
		
		//Create child actor component to store our wire actor. This should probably be optimized as it's a lot better for performance to just have 1 actor that handles all the wires.
		auto WireActorComponent = Cast<UChildActorComponent>(AddComponentByClass(UChildActorComponent::StaticClass(), false, FTransform::Identity, false));
		WireActorComponent->SetIsReplicated(true);
		WireActorComponent->SetChildActorClass(WireClass);
		WireActorComponent->CreateChildActor();
		auto WireActor = Cast<AConnectionWireBase>(WireActorComponent->GetChildActor());


		//Create data for the wire
		auto InputInfo = InputInfoList[i];
		FConnectionData ReceiverData = FConnectionData(this, InputInfo.FunctionName, InputInfo.ConnectionType);
		ReceiverData.DisplayName = InputInfo.DisplayedName;

		//Assign data to the wire
		FDynamicConnectionData AssignedConnection = FDynamicConnectionData();
		AssignedConnection.Transmitter = ConnectionData;
		AssignedConnection.Receiver = ReceiverData;
		WireActor->AssignedConnection = AssignedConnection;
		WireActor->DrawWireFromData();
		
		WireActorComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
		WireActorComponent->SetHiddenInGame(false);
	}
	
}

void AFGWiremodBuildable::PostGateSetup_Implementation(){}
void AFGWiremodBuildable::SetupGate_Implementation()
{
	if(HasAuthority())
		DrawWires();
}
