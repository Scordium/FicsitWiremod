
#include "Behaviour/VanillaInterface/WiremodVanillaConnections.h"


void AWiremodVanillaConnections::DrawWiresForBuildable(FVanillaBuildingDataKeyValuePair KeyValuePair, bool SkipDestruct)
{

	AActor* Buildable = Cast<AActor>(KeyValuePair.Buildable);
	if(!Buildable) return;
	
	//Remove all existing wires
	if(!SkipDestruct)
	{
		TArray<UActorComponent*> ChildComponents;
		Buildable->GetComponents(UChildActorComponent::StaticClass(), ChildComponents);
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
	}

	//If there are no wires to draw then exit the function
	if(KeyValuePair.Data.Connections.Num() == 0) return;

	//Visible wire class to use as wire
	auto WireClass = AConnectionWireBase::StaticClass();
	
	//Create new wires
	for (auto Connection : KeyValuePair.Data.Connections)
	{
		if(!Connection.IsValidForWire()) continue;
				
		//Create child actor component to store our wire actor. This should probably be optimized as it's a lot better for performance to just have 1 actor that handles all the wires.
		auto WireActorComponent = Cast<UChildActorComponent>(Buildable->AddComponentByClass(UChildActorComponent::StaticClass(), false, FTransform::Identity, false));
		WireActorComponent->SetIsReplicated(true);
		WireActorComponent->SetChildActorClass(WireClass);
		WireActorComponent->CreateChildActor();
		auto WireActor = Cast<AConnectionWireBase>(WireActorComponent->GetChildActor());
		WireActor->SetActorTickEnabled(true);

		//Assign data to wire
		WireActor->DrawWireFromData(Connection);
				
		WireActorComponent->AttachToComponent(Buildable->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
		WireActorComponent->SetHiddenInGame(false);
	}
}

void AWiremodVanillaConnections::Tick(float DeltaTime)
{
	if(!HasAuthority()) return;

	bool FilterNulls = false;
	TArray<int> ClearIndexes;
	
	for(int i = 0; i < SavedVanillaBuildableData.Num(); i++)
	{
		auto Data = SavedVanillaBuildableData[i];
		
		if(!Data.Buildable)
		{
			FilterNulls = true;
			continue;
		}
			
		if(Data.Data.Connections.Num() == 0) continue;
		
		if(!HandleDynamicConnections(Data.Data.Connections))
		{
			FilterNulls = true;
			ClearIndexes.Add(i);
		}
	}

	if(FilterNulls) FilterNullPointers(ClearIndexes);
}


bool AWiremodVanillaConnections::HandleDynamicConnections(TArray<FDynamicConnectionData> connections)
{
	bool HadValid = false;
	for (auto ConnectionData : connections)
	{
		bool HasNullPtr = !ConnectionData.Transmitter.Object || !ConnectionData.Receiver.Object;
		if(HasNullPtr) continue;

		HadValid = true;
			
		switch (ConnectionData.Receiver.ConnectionType)
		{
		case Boolean: WM::SetFunctionBoolValue(ConnectionData.Receiver, WM::GetFunctionBoolResult(ConnectionData.Transmitter)); break;
		case Number:
		case Integer: WM::SetFunctionNumberValue(ConnectionData.Receiver, WM::GetFunctionNumberResult(ConnectionData.Transmitter)); break;
		case String: WM::SetFunctionStringValue(ConnectionData.Receiver, WM::GetFunctionStringResult(ConnectionData.Transmitter)); break;
		case Color: WM::SetFunctionColorValue(ConnectionData.Receiver, WM::GetFunctionColorResult(ConnectionData.Transmitter)); break;
		case Recipe: WM::SetFunctionRecipeValue(ConnectionData.Receiver, WM::GetFunctionRecipeResult(ConnectionData.Transmitter)); break;
		default: break;
		}
	}

	return HadValid;
}

void AWiremodVanillaConnections::FilterNullPointers(TArray<int> ClearIndexes)
{
	TArray<FVanillaBuildingDataKeyValuePair> Filtered;

	for(int i = 0; i < SavedVanillaBuildableData.Num(); i++)
	{
		auto Data = SavedVanillaBuildableData[i];
		if(!Data.Buildable || ClearIndexes.Contains(i)) continue;
		Filtered.Add(Data);
	}

	SavedVanillaBuildableData = Filtered;
	OnRep_SavedVanillaData();
}