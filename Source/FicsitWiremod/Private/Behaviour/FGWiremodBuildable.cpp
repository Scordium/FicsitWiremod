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
#include "Kismet/KismetMaterialLibrary.h"

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
	DOREPLIFETIME(AFGWiremodBuildable, BreadboardReference);
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
	return GetConnections_Implementation(direction);
}

TArray<FBuildingConnection> AFGWiremodBuildable::GetConnections_Implementation(EConnectionDirection direction)
{
	FBuildableNote Note;
	switch (direction)
	{
	case Input: return ConnectionsInfo.Inputs;
	case Output: return ConnectionsInfo.Outputs;
		//Possibly add more cases here for other behaviors?
	default: return {};
	}
}

TSubclassOf<UUserWidget> AFGWiremodBuildable::GetCompactWidget_Implementation()
{
	return CompactWidget;
}


int AFGWiremodBuildable::netFunc_getFunctionReturnType(FString FunctionName)
{
	for(const auto& Connection : Execute_GetConnections(this, Output))
	{
		bool IsMatch = Connection.FunctionName.ToString().Equals(FunctionName, ESearchCase::IgnoreCase) || Connection.DisplayedName.ToString().Equals(FunctionName, ESearchCase::IgnoreCase);
		if(IsMatch)
			return Connection.ConnectionType.GetValue();
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
void AFGWiremodBuildable::netFuncMeta_setBool(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setBool";
	DisplayName = FText::FromString("Set Bool");
	Description = FText::FromString("Sets a Bool parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setBoolArray(int InputIndex, TArray<bool> Value)
{
	auto InputObject = NewObject<UCCBoolArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setBoolArray(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setBoolArray";
	DisplayName = FText::FromString("Set Bool Array");
	Description = FText::FromString("Sets a Bool Array parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setNumber(int InputIndex, double Value)
{
	auto InputObject = NewObject<UCCNumberValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setNumber(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setNumber";
	DisplayName = FText::FromString("Set Number");
	Description = FText::FromString("Sets a Number parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setNumberArray(int InputIndex, TArray<double> Value)
{
	auto InputObject = NewObject<UCCNumberArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setNumberArray(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setNumberArray";
	DisplayName = FText::FromString("Set Number Array");
	Description = FText::FromString("Sets a Number Array parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setString(int InputIndex, FString Value)
{
	auto InputObject = NewObject<UCCStringValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setString(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setString";
	DisplayName = FText::FromString("Set String");
	Description = FText::FromString("Sets a String parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setStringArray(int InputIndex, TArray<FString> Value)
{
	auto InputObject = NewObject<UCCStringArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setStringArray(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setStringArray";
	DisplayName = FText::FromString("Set String Array");
	Description = FText::FromString("Sets a String Array parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setVector(int InputIndex, FVector Value)
{
	auto InputObject = NewObject<UCCVectorValue>(this);
	InputObject->Value = Value;
	

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setVector(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setVector";
	DisplayName = FText::FromString("Set Vector");
	Description = FText::FromString("Sets a Vector parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setVectorArray(int InputIndex, TArray<FVector> Value)
{
	auto InputObject = NewObject<UCCVectorArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setVectorArray(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setVectorArray";
	DisplayName = FText::FromString("Set Vector Array");
	Description = FText::FromString("Sets a Vector Array parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setInventory(int InputIndex, UFGInventoryComponent* Value)
{
	auto InputObject = NewObject<UCCInventoryValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setInventory(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setInventory";
	DisplayName = FText::FromString("Set Inventory");
	Description = FText::FromString("Sets an Inventory parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setInventoryArray(int InputIndex, TArray<UFGInventoryComponent*> Value)
{
	auto InputObject = NewObject<UCCInventoryArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setInventoryArray(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setInventoryArray";
	DisplayName = FText::FromString("Set Inventory Array");
	Description = FText::FromString("Sets an Inventory Array parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setPowerGrid(int InputIndex, UFGPowerCircuit* Value)
{
	auto InputObject = NewObject<UCCCircuitValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setPowerGrid(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setPowerGrid";
	DisplayName = FText::FromString("Set Power Grid");
	Description = FText::FromString("Sets a Power Grid parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setPowerGridArray(int InputIndex, TArray<UFGPowerCircuit*> Value)
{
	auto InputObject = NewObject<UCCCircuitArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setPowerGridArray(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setPowerGridArray";
	DisplayName = FText::FromString("Set Power Grid Array");
	Description = FText::FromString("Sets a Power Grid Array parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setEntity(int InputIndex, AActor* Value)
{
	auto InputObject = NewObject<UCCEntityValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setEntity(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setEntity";
	DisplayName = FText::FromString("Set Entity");
	Description = FText::FromString("Sets an Entity parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setEntityArray(int InputIndex, TArray<AActor*> Value)
{
	auto InputObject = NewObject<UCCEntityArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setEntityArray(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setEntityArray";
	DisplayName = FText::FromString("Set Entity Array");
	Description = FText::FromString("Sets an Entity Array parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setRecipe(int InputIndex, TSubclassOf<UFGRecipe> Value)
{
	auto InputObject = NewObject<UCCRecipeValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setRecipe(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setRecipe";
	DisplayName = FText::FromString("Set Recipe");
	Description = FText::FromString("Sets a Recipe parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setRecipeArray(int InputIndex, TArray<TSubclassOf<UFGRecipe>> Value)
{
	auto InputObject = NewObject<UCCRecipeArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setRecipeArray(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setRecipeArray";
	DisplayName = FText::FromString("Set Recipe Array");
	Description = FText::FromString("Sets a Recipe Array parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setColor(int InputIndex, FLinearColor Value)
{
	auto InputObject = NewObject<UCCColorValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setColor(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setColor";
	DisplayName = FText::FromString("Set Color");
	Description = FText::FromString("Sets a Color parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setColorArray(int InputIndex, TArray<FLinearColor> Value)
{
	auto InputObject = NewObject<UCCColorArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setColorArray(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setColorArray";
	DisplayName = FText::FromString("Set Color Array");
	Description = FText::FromString("Sets a Color Array parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setStack(int InputIndex, FInventoryStack Value)
{
	auto InputObject = NewObject<UCCStackValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setStack(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setStack";
	DisplayName = FText::FromString("Set Stack");
	Description = FText::FromString("Sets a Stack parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setStackArray(int InputIndex, TArray<FInventoryStack> Value)
{
	auto InputObject = NewObject<UCCStackArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setStackArray(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setStackArray";
	DisplayName = FText::FromString("Set Stack Array");
	Description = FText::FromString("Sets a Stack Array parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setItemAmount(int InputIndex, FItemAmount Value)
{
	auto InputObject = NewObject<UCCItemAmountValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setItemAmount(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setItemAmount";
	DisplayName = FText::FromString("Set Item Amount");
	Description = FText::FromString("Sets an Item Amount parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
}

void AFGWiremodBuildable::netFunc_setItemAmountArray(int InputIndex, TArray<FItemAmount> Value)
{
	auto InputObject = NewObject<UCCItemAmountArrayValue>(this);
	InputObject->Value = Value;

	auto InputData = FConnectionData(InputObject, FName("Value"), InputObject->ConnectionType.GetValue());
	Execute_OnInputConnected(this, InputData, InputIndex, nullptr);
}
void AFGWiremodBuildable::netFuncMeta_setItemAmountArray(FString& InternalName, FText& DisplayName, FText& Description, TArray<FString>& ParameterInternalNames, TArray<FText>& ParameterDisplayNames, TArray<FText>& ParameterDescriptions, int32& Runtime)
{
	InternalName = "setItemAmountArray";
	DisplayName = FText::FromString("Set Item Amount Array");
	Description = FText::FromString("Sets an Item Amount Array parameter");
	ParameterInternalNames.Add("InputIndex");
	ParameterDisplayNames.Add(FText::FromString("Input Index"));
	ParameterDescriptions.Add(FText::FromString("Index of the Parameter"));
	ParameterInternalNames.Add("Value");
	ParameterDisplayNames.Add(FText::FromString("Value"));
	ParameterDescriptions.Add(FText::FromString("Value of the Parameter"));
	Runtime = 0;
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

void AFGWiremodBuildable::UpdateObjectDecalTexture()
{
	auto Texture = GetTexture();
	UMaterialInstanceDynamic* Material;
	
	//Get material
	auto DecalMaterial = Decal->GetMaterial(0);
	if(auto DynamicInstance = Cast<UMaterialInstanceDynamic>(DecalMaterial)) Material = DynamicInstance;
	else Material = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), DecalMaterial);

	//Set material parameters
	if(!Texture)
	{
		Material->SetScalarParameterValue(FName("OpacityMultiplier"), 0);
	}
	else
	{
		Material->SetTextureParameterValue(FName("Texture"), Texture);
		Material->SetScalarParameterValue(FName("OpacityMultiplier"), 1);
	}
	Decal->SetMaterial(0, Material);
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
	for (int i = 0; i < InputInfoList.Num(); i++)
	{
		FConnectionData ConnectionData = GetConnection(i);
		if(!ConnectionData.IsValidForWire()) continue;
		
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
	if(HasAuthority()) DrawWires();
	
	UpdateObjectDecalTexture();
}
