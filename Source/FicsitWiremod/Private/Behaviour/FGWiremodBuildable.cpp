// 
#include "Behaviour/FGWiremodBuildable.h"

#include "WiremodReflection.h"
#include "Buildables/FGBuildableBlueprintDesigner.h"
#include "CommonLib/DynamicValues/CCBoolValue.h"
#include "CommonLib/DynamicValues/CCCircuitValue.h"
#include "CommonLib/DynamicValues/CCColorValue.h"
#include "CommonLib/DynamicValues/CCEntityValue.h"
#include "CommonLib/DynamicValues/CCInventoryValue.h"
#include "CommonLib/DynamicValues/CCItemAmountValue.h"
#include "CommonLib/DynamicValues/CCNumberValue.h"
#include "CommonLib/DynamicValues/CCRecipeValue.h"
#include "CommonLib/DynamicValues/CCSplitterRuleValue.h"
#include "CommonLib/DynamicValues/CCStackValue.h"
#include "CommonLib/DynamicValues/CCStringValue.h"
#include "CommonLib/DynamicValues/CCVectorValue.h"

void AFGWiremodBuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(HasAuthority()) ServerProcess(DeltaTime);
	ClientProcess(DeltaTime);
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
	if(Index == -1) InputConnections.Empty();
	else
	{
		if(InputConnections.IsValidIndex(Index))
			InputConnections[Index] = FConnectionData();
	}
	
	DrawWires();
}


TArray<FBuildingConnection> AFGWiremodBuildable::GetConnectionsInfo_Implementation(EConnectionDirection direction, FBuildableNote& Note)
{
	Note = ConnectionsInfo.Note;
	
	switch (direction)
	{
	case Input: return ConnectionsInfo.Inputs;
	case Output: return ConnectionsInfo.Outputs;
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
	for(auto conn : Execute_GetConnections(this, Output))
	{
		bool IsMatch = conn.FunctionName.ToString().Equals(FunctionName, ESearchCase::IgnoreCase) || conn.DisplayedName.ToString().Equals(FunctionName, ESearchCase::IgnoreCase);
		if(IsMatch)
			return conn.ConnectionType.GetValue();
	}

	return -1;
}

void AFGWiremodBuildable::netFunc_setBool(int InputIndex, bool Value)
{
	auto InputObject = NewObject<UCCBoolValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setBoolArray(int InputIndex, TArray<bool> Value)
{
	auto InputObject = NewObject<UCCBoolArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setNumber(int InputIndex, double Value)
{
	auto InputObject = NewObject<UCCNumberValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setNumberArray(int InputIndex, TArray<double> Value)
{
	auto InputObject = NewObject<UCCNumberArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setString(int InputIndex, FString Value)
{
	auto InputObject = NewObject<UCCStringValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setStringArray(int InputIndex, TArray<FString> Value)
{
	auto InputObject = NewObject<UCCStringArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setVector(int InputIndex, FVector Value)
{
	auto InputObject = NewObject<UCCVectorValue>(this);
	InputObject->Value = Value;
	

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setVectorArray(int InputIndex, TArray<FVector> Value)
{
	auto InputObject = NewObject<UCCVectorArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setInventory(int InputIndex, UFGInventoryComponent* Value)
{
	auto InputObject = NewObject<UCCInventoryValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setInventoryArray(int InputIndex, TArray<UFGInventoryComponent*> Value)
{
	auto InputObject = NewObject<UCCInventoryArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setPowerGrid(int InputIndex, UFGPowerCircuit* Value)
{
	auto InputObject = NewObject<UCCCircuitValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setPowerGridArray(int InputIndex, TArray<UFGPowerCircuit*> Value)
{
	auto InputObject = NewObject<UCCCircuitArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setEntity(int InputIndex, AActor* Value)
{
	auto InputObject = NewObject<UCCEntityValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setEntityArray(int InputIndex, TArray<AActor*> Value)
{
	auto InputObject = NewObject<UCCEntityArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setRecipe(int InputIndex, TSubclassOf<UFGRecipe> Value)
{
	auto InputObject = NewObject<UCCRecipeValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setRecipeArray(int InputIndex, TArray<TSubclassOf<UFGRecipe>> Value)
{
	auto InputObject = NewObject<UCCRecipeArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setColor(int InputIndex, FLinearColor Value)
{
	auto InputObject = NewObject<UCCColorValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setColorArray(int InputIndex, TArray<FLinearColor> Value)
{
	auto InputObject = NewObject<UCCColorArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setStack(int InputIndex, FInventoryStack Value)
{
	auto InputObject = NewObject<UCCStackValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setStackArray(int InputIndex, TArray<FInventoryStack> Value)
{
	auto InputObject = NewObject<UCCStackArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setItemAmount(int InputIndex, FItemAmount Value)
{
	auto InputObject = NewObject<UCCItemAmountValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

void AFGWiremodBuildable::netFunc_setItemAmountArray(int InputIndex, TArray<FItemAmount> Value)
{
	auto InputObject = NewObject<UCCItemAmountArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}

bool AFGWiremodBuildable::netFunc_getBool(FString FunctionName, bool DefaultValue){ return FConnectionData(this, FName(FunctionName), Boolean).GetBool(DefaultValue); }
double AFGWiremodBuildable::netFunc_getNumber(FString FunctionName, double DefaultValue) { return FConnectionData(this, FName(FunctionName), Number).GetFloat(DefaultValue); }
FString AFGWiremodBuildable::netFunc_getString(FString FunctionName, FString DefaultValue) { return FConnectionData(this, FName(FunctionName), String).GetString(DefaultValue); }
FVector AFGWiremodBuildable::netFunc_getVector(FString FunctionName, FVector DefaultValue) { return FConnectionData(this, FName(FunctionName), Vector).GetVector(DefaultValue); }
FLinearColor AFGWiremodBuildable::netFunc_getColor(FString FunctionName, FLinearColor DefaultValue) { return FConnectionData(this, FName(FunctionName), Color).GetColor(DefaultValue); }
FString AFGWiremodBuildable::netFunc_getOutputString(FString FunctionName) { return FConnectionData(this, FName(FunctionName), (EConnectionType) netFunc_getFunctionReturnType(FunctionName)).GetStringifiedValue(); }


FString AFGWiremodBuildable::netFunc_getInputValueString(int InputIndex)
{
	return GetConnection(InputIndex).GetStringifiedValue();
}

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

void AFGWiremodBuildable::netFunc_getAllCircuitryFuncs(TArray<FString>& Out)
{
	for(auto& Connection : GetConnections_Implementation(Output))
	{
		Out.Add(Connection.FunctionName.ToString());
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
