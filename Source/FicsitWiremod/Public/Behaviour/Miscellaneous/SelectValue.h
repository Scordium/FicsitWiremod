// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "SelectValue.generated.h"

UCLASS()
class FICSITWIREMOD_API ASelectValue : public AMultistateWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		GenerateInputList();
		
		//Default mode (aka IF TRUE RETURN TRUE_VAL ELSE RETURN FALSE_VAL)
		if(CurrentStateIndex == 0)
		{
			if(GetConnection(0).GetBool())
				Out = UCCDynamicValueUtils::FromValue(GetConnection(2), Out ? Out->GetWorld() : this->GetWorld());
			else
				Out = UCCDynamicValueUtils::FromValue(GetConnection(1), Out ? Out->GetWorld() : this->GetWorld());
		}
		//Alt mode - Select one from a list
		else if (CurrentStateIndex == 1)
		{
			int Value = GetConnection(0).GetFloat() + 1;
			if(IsConnected(Value))
				Out = UCCDynamicValueUtils::FromValue(GetConnection(Value), Out ? Out->GetWorld() : this->GetWorld());
			else
				Out = nullptr;
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ASelectValue, Out)
	}

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override
	{
		bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
		WroteSomething |= Channel->ReplicateSubobject(Out, *Bunch, *RepFlags);

		return WroteSomething;
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override{ return Out; }

	int FindSelectorIndex()
	{
		if(InputConnections.Num() <= 1) return INDEX_NONE;

		for(int i = InputConnections.Num() - 1; i > 0; i--)
		{
			if(InputConnections[i].IsValid()) return i;
		}

		return INDEX_NONE;
	}

// Why? Why not :^)
#define MinSelectorInputs 8
	
	void GenerateInputList()
	{
		//Default mode (aka IF TRUE RETURN TRUE_VAL ELSE RETURN FALSE_VAL)
		if(CurrentStateIndex == 0)
		{
			//Prioritize FALSE_VAL input, if it's connected then use its connection type for both value inputs and the output
			if(IsConnected(1))
			{
				SetInputType(1, GetConnection(1).ConnectionType);
				SetInputType(2, GetConnection(1).ConnectionType);
				SetOutputType(0, GetConnection(1).ConnectionType);
			}
			//If FALSE_VAL is not connected, then try to do the same with TRUE_VAL input
			else if(IsConnected(2))
			{
				SetInputType(1, GetConnection(2).ConnectionType);
				SetInputType(2, GetConnection(2).ConnectionType);
				SetOutputType(0, GetConnection(2).ConnectionType);
			}
			//If both value inputs are not connected, then set inputs to "Any" type and output to "Unknown"
			else
			{
				SetInputType(1, Any);
				SetInputType(2, Any);
				SetOutputType(0, Unknown);
			}
		}
		//Alt mode - Select one from a list
		else
		{
			auto FoundIndex = FindSelectorIndex();
			auto Type = FoundIndex > 0 ? GetConnection(FoundIndex).ConnectionType.GetValue() : Any;

			//I don't even know what i'm doing at this point, but all this shit basically prevents selectors from pointlessly recreating their connection list.
			//I'd prefer to just do it on input connect/disconnect, but then cases when the output building was destroyed remain unhandled.
			bool NotInitialized = FoundIndex == INDEX_NONE && ConnectionsInfo.Inputs.Num() == 0;

			//If all selector inputs are disconnected, do we need to reset them to default mode so they accept "Any" value type?
			bool NotResetToDefault = Type == Any && GetInputType(1) != Any;

			//If any selector input is connected, do we need to update the connection type for all inputs?
			bool NotSetToInputType = Type != GetOutputType(0) && Type != Any;

			//Whether we need to recreate selector connections
			bool UpdateInputTypes = NotResetToDefault || NotSetToInputType;

			//Input count is not equal to the index of the last connected input in the list,
			//and the index is greater than the minimum amount of connectors the selector must have
			//and index is not default value (-1)
			bool UpdateInputList = ConnectionsInfo.Inputs.Num() != FoundIndex && FoundIndex >= MinSelectorInputs && FoundIndex != INDEX_NONE;
			
			if(UpdateInputTypes || UpdateInputList || NotInitialized)
				MakeSelectorConnections(ConnectionsInfo.Inputs, FoundIndex >= MinSelectorInputs ? FoundIndex : MinSelectorInputs, Type);

			SetOutputType(0, Type == Any ? Unknown : Type);
		}
	}

	void MakeSelectorConnections(TArray<FBuildingConnection>& OutArray, int Count = MinSelectorInputs, EConnectionType Type = Any)
	{
		OutArray.Empty();
		OutArray.Add(FBuildingConnection("Value", "", Integer));
		for(int i = 0; OutArray.Num() < Count; i++)
		{
			OutArray.Add(FBuildingConnection(FString::FromInt(i), "", Type));
		}
	}


	UPROPERTY(Replicated)
	UCCDynamicValueBase* Out;
};
