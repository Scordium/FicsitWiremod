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
			UWiremodReflection::FillDynamicStructFromData(GetConnection(Value), Out);
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ASelectValue, Out)
	}

	virtual FDynamicValue GetValue_Implementation(const FString& ValueName) override{ return Out; }


	void GenerateInputList()
	{
		
		//Default mode (aka IF TRUE RETURN TRUE_VAL ELSE RETURN FALSE_VAL)
		if(CurrentStateIndex == 0)
		{
			FBuildingConnection SelectorInput = FBuildingConnection("Value", "", Boolean);
			FBuildingConnection FalseValueInput = FBuildingConnection("False", "", Any);
			FBuildingConnection TrueValueInput = FBuildingConnection("True", "", Any);


			//Prioritize FALSE_VAL input, if it's connected then use its connection type for both value inputs
			if(IsConnected(1))
			{
				FalseValueInput.ConnectionType = GetConnection(1).ConnectionType;
				TrueValueInput.ConnectionType = GetConnection(1).ConnectionType;
			}
			//If FALSE_VAL is not connected, then try to do the same with TRUE_VAL input
			else if(IsConnected(2))
			{
				FalseValueInput.ConnectionType = GetConnection(2).ConnectionType;
				TrueValueInput.ConnectionType = GetConnection(2).ConnectionType;
			}

			//If both value inputs are not connected, then we don't need to do anything since we already set the inputs to accept "Any" type.
			

			//Make an array and assign it to inputs.
			ConnectionsInfo.Inputs = TArray
			{
				SelectorInput,
				FalseValueInput,
				TrueValueInput
			};
		}
		//Alt mode - Select one from a list
		else
		{
			for(int i = InputConnections.Num(); i > 0; i--)
			{
				//If there's an input already connected, then we should use it's connection type and update all other inputs as well.
				if(IsConnected(i))
				{
					ConnectionsInfo.Inputs = MakeSelectorConnections(i, GetConnection(i).ConnectionType);
					return;
				}
			}

			//If no inputs are connected, then use the default mode - set all inputs to "Any" connection type, and wait for user to connect something.
			ConnectionsInfo.Inputs = MakeSelectorConnections();
		}
	}

	static TArray<FBuildingConnection> MakeSelectorConnections(int Count = 8, EConnectionType Type = Any)
	{
		auto Out = TArray
		{
			FBuildingConnection("Value", "", Integer)
		};

		for(int i = 0; i <= Count; i++)
		{
			Out.Add(FBuildingConnection(FString::FromInt(i), "", Type));
		}

		return Out;
	}


	UPROPERTY(Replicated)
	FDynamicValue Out;
};
