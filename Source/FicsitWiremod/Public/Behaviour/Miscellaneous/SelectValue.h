// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "SelectValue.generated.h"

UCLASS()
class FICSITWIREMOD_API ASelectValue : public AMultistateWiremodBuildable, public IIConstantsDistributor
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		GenerateInputList();
		
		//Default mode (aka IF TRUE RETURN TRUE_VAL ELSE RETURN FALSE_VAL)
		if(CurrentStateIndex == 0)
		{
			if(WM_GetBool(0))
				UWiremodReflection::FillDynamicStructFromData(GetConnection(2), Out);
			else
				UWiremodReflection::FillDynamicStructFromData(GetConnection(1), Out);
		}
		//Alt mode - Select one from a list
		else if (CurrentStateIndex == 1)
		{
			int Value = WM_GetInt(0) + 1;
			if(IsConnected(Value))
				UWiremodReflection::FillDynamicStructFromData(GetConnection(Value), Out);
			else
				Out = FDynamicValue();
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ASelectValue, Out)
	}

	virtual FDynamicValue GetValue_Implementation(const FString& ValueName) override{ return Out; }

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
			auto Type = FoundIndex > 0 ? GetConnection(FoundIndex).ConnectionType : Any;

			//I don't even know what i'm doing at this point, but all this shit basically prevents selectors from pointlessly recreating their connection list.
			//I'd prefer to just do it on input connect/disconnect, but then cases when the output building was destroyed remain unhandled.
			bool NotInitialized = FoundIndex == INDEX_NONE && ConnectionsInfo.Inputs.Num() == 0;
			
			bool NotResetToDefault = Type == Any && GetInputType(1) != Any;
			bool NotSetToInputType = Type != GetOutputType(0) && Type != Any;
			bool UpdateInputTypes = NotResetToDefault || NotSetToInputType;
			
			bool UpdateInputList = ConnectionsInfo.Inputs.Num() != FoundIndex && FoundIndex != INDEX_NONE && FoundIndex >= MinSelectorInputs;
			
			if(UpdateInputTypes || UpdateInputList || NotInitialized)
				MakeSelectorConnections(ConnectionsInfo.Inputs, FoundIndex >= MinSelectorInputs ? FoundIndex : MinSelectorInputs, Type);
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

		SetOutputType(0, Type == Any ? Unknown : Type);
	}


	UPROPERTY(Replicated)
	FDynamicValue Out;
};
