// 
#include "Behaviour/FGWiremodBuildable.h"

void AFGWiremodBuildable::OnInputConnected_Implementation(FNewConnectionData Data, int Index, UObject* Setter)
{
	if(!GetCanConnect(Setter)) return;

	if(InputConnections.Num() <= Index)
		InputConnections.SetNum(Index + 1);
	
	InputConnections[Index] = Data;

	DrawWires();
}

void AFGWiremodBuildable::OnInputDisconnected_Implementation(int Index, UObject* Setter)
{
	if(!GetCanDisconnect(Setter)) return;

	if(InputConnections.IsValidIndex(Index))
		InputConnections[Index] = FNewConnectionData();

	DrawWires();
}

TArray<FBuildingConnection> AFGWiremodBuildable::GetAvailableConnections_Implementation(EConnectionDirection direction)
{
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
