// 
#include "Behaviour/FGWiremodBuildable.h"

#include "WiremodUtils.h"

void AFGWiremodBuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsActiveEntity()) Process(DeltaTime);
}

FNewConnectionData AFGWiremodBuildable::GetConnection(int Index)
{
	if(!InputConnections.IsValidIndex(Index)) return FNewConnectionData(nullptr, FName());
	return InputConnections[Index];
}

void AFGWiremodBuildable::GetAllConnected(TArray<FNewConnectionData>& Out)
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

void AFGWiremodBuildable::OnInputConnected_Implementation(const FNewConnectionData& Data, int Index, UObject* Setter)
{
	if(!GetCanConnect(Setter)) return;
	OnInputConnected_Internal(Data, Index);
}

void AFGWiremodBuildable::OnInputConnected_Internal(const FNewConnectionData& Data, int Index)
{
	if(InputConnections.Num() <= Index)
		InputConnections.SetNum(Index + 1);

	auto ConvertedPositions = ConvertConnections(Data.WirePositions);

	//A small hack to force redraw the wire if it doesn't have a complex shape. Implemented for usage with mods that can move buildables (i.e. MicroManage)
	if(ConvertedPositions.Num() < 3) ConvertedPositions.Empty();
	InputConnections[Index] = FNewConnectionData(Data, ConvertedPositions);
	
	DrawWires();
}





void AFGWiremodBuildable::OnInputDisconnected_Implementation(int Index, UObject* Setter)
{
	if(!GetCanDisconnect(Setter)) return;
	OnInputDisconnected_Internal(Index);
}

void AFGWiremodBuildable::OnInputDisconnected_Internal(int Index)
{
	if(Index == -1){ InputConnections.Empty(); }
	else
	{
		if(InputConnections.IsValidIndex(Index))
			InputConnections[Index] = FNewConnectionData();
	}
	
	DrawWires();
}


TArray<FBuildingConnection> AFGWiremodBuildable::GetAvailableConnections_Implementation(EConnectionDirection direction, int& Count, FBuildableNote& Note)
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
	for(auto conn : GetAvailableConnections_Slim(Input))
	{
		if(conn.FunctionName.ToString() == FunctionName || conn.DisplayName == FunctionName)
			return conn.ConnectionType.GetValue();
	}

	return -1;
}
bool AFGWiremodBuildable::netFunc_getWireBool(FString FunctionName, bool DefaultValue){ return UWiremodReflection::GetFunctionBoolResult(FNewConnectionData(this, FName(FunctionName), Boolean), DefaultValue); }
float AFGWiremodBuildable::netFunc_getWireNumber(FString FunctionName, float DefaultValue) { return UWiremodReflection::GetFunctionNumberResult(FNewConnectionData(this, FName(FunctionName), Number), DefaultValue); }
FString AFGWiremodBuildable::netFunc_getWireString(FString FunctionName, FString DefaultValue) { return UWiremodReflection::GetFunctionStringResult(FNewConnectionData(this, FName(FunctionName), String), DefaultValue); }
FVector AFGWiremodBuildable::netFunc_getWireVector(FString FunctionName, FVector DefaultValue) { return UWiremodReflection::GetFunctionVectorResult(FNewConnectionData(this, FName(FunctionName), Vector), DefaultValue); }
FLinearColor AFGWiremodBuildable::netFunc_getWireColor(FString FunctionName, FLinearColor DefaultValue) { return UWiremodReflection::GetFunctionColorResult(FNewConnectionData(this, FName(FunctionName), Color), DefaultValue); }
FString AFGWiremodBuildable::netFunc_getDebuggerOutputString(FString FunctionName) { return UWiremodUtils::GetStringifiedValue(FNewConnectionData(this, FName(FunctionName), (EConnectionType) netFunc_getFunctionReturnType(FunctionName))); }

void AFGWiremodBuildable::GetInputOccupationStatus(EConnectionType AllowedType, TArray<TEnumAsByte<EConnectionOccupationState>>& Out)
{
	auto Inputs = GetAvailableConnections_Slim(Input);

	for(int i = 0; i < Inputs.Num(); i++)
	{
		auto Connection = GetConnection(i);
		if(!Connection.Object)
		{
			bool ValidPair = UWiremodUtils::IsValidConnectionPair(Inputs[i].ConnectionType, AllowedType);
			Out.Add(ValidPair ? Free : Disabled);
		}
		else
		{
			if(false /*Connection.Object.IsA<GlobalConnectionDistributor>()*/) Out.Add(GlobalConnection);
			else Out.Add(Connected);
		}
	}
}

void AFGWiremodBuildable::netFunc_getAllWiremodFuncs(TArray<FString>& Out)
{
	for(auto conn : GetAvailableConnections_Slim(Input))
	{
		Out.Add(conn.FunctionName.ToString());
	}
}

void AFGWiremodBuildable::DrawWires_Implementation()
{
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
	auto InputInfoList = GetAvailableConnections_Slim(Input);
	if(InputInfoList.Num() == 0) return;

	//Visible wire class to use as wire
	auto WireClass = Assets->VisibleWireClass;
	
	//Create new wires
	for (int i = 0; i < InputConnections.Num(); i++)
	{
		FNewConnectionData ConnectionData = GetConnection(i);
		if(!ConnectionData.IsValid()) continue;
		if(ConnectionData.WireHidden) continue;


		//Create child actor component to store our wire actor. This should probably be optimized as it's a lot better for performance to just have 1 actor that handles all the wires.
		auto WireActorComponent = Cast<UChildActorComponent>(AddComponentByClass(UChildActorComponent::StaticClass(), false, FTransform::Identity, false));
		WireActorComponent->SetIsReplicated(true);
		WireActorComponent->SetChildActorClass(WireClass);
		WireActorComponent->CreateChildActor();
		auto WireActor = Cast<AConnectionWireBase>(WireActorComponent->GetChildActor());
		WireActor->SetActorTickEnabled(true);


		//Create data for the wire
		auto InputInfo = InputInfoList[i];
		FNewConnectionData ReceiverData = FNewConnectionData(this, InputInfo.FunctionName, InputInfo.ConnectionType);
		ReceiverData.DisplayName = InputInfo.DisplayName;

		//Assign data to the wire
		FDynamicConnectionData AssignedConnection = FDynamicConnectionData();
		AssignedConnection.Transmitter = ConnectionData;
		AssignedConnection.Receiver = ReceiverData;
		WireActor->DrawWire(AssignedConnection);
		
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
