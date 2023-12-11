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
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
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

		SetOutputType(0, Out ? Out->ConnectionType.GetValue() : Unknown);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ASelectValue, Out)
	}

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& OutObjects) override
	{
		Super::GatherReplicatedObjects_Implementation(OutObjects);
		OutObjects.Add(Out);
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override{ return Out; }

	virtual TArray<FBuildingConnection> GetConnectionsInfo_Implementation(EConnectionDirection direction, FBuildableNote& Note) override
	{
		if(direction == Output) return Super::GetConnectionsInfo_Implementation(direction, Note);
		else
		{
			auto Connections = GenerateInputList();
			return Connections;
		}
	}
	
	
	TArray<FBuildingConnection> GenerateInputList()
	{
		auto SelectionInput = States[CurrentStateIndex].Connections.Inputs[0];
		//Default mode (aka IF TRUE RETURN TRUE_VAL ELSE RETURN FALSE_VAL)
		if(CurrentStateIndex == 0)
		{
			auto InputFalse = States[CurrentStateIndex].Connections.Inputs[1];
			auto InputTrue = States[CurrentStateIndex].Connections.Inputs[2];
			auto OutArray = TArray
			{
				FBuildingConnection(SelectionInput.DisplayedName, "", SelectionInput.ConnectionType),
				FBuildingConnection(InputFalse.DisplayedName, "", Any),
				FBuildingConnection(InputTrue.DisplayedName, "", Any)
			};
			//Prioritize FALSE_VAL input, if it's connected then use its connection type for both value inputs and the output
			if(IsConnected(1))
			{
				OutArray[1].ConnectionType = GetConnection(1).ConnectionType;
				OutArray[2].ConnectionType = GetConnection(1).ConnectionType;
			}
			//If FALSE_VAL is not connected, then try to do the same with TRUE_VAL input
			else if(IsConnected(2))
			{
				OutArray[1].ConnectionType = GetConnection(2).ConnectionType;
				OutArray[2].ConnectionType = GetConnection(2).ConnectionType;
			}

			return OutArray;
		}
		//Alt mode - Select one from a list
		else
		{
			auto FoundIndex = FindSelectorIndex();
			auto Type = GetConnection(FoundIndex).IsValid() ? GetConnection(FoundIndex).ConnectionType.GetValue() : Any;
			FoundIndex = FoundIndex >= 8 ? FoundIndex : 8;
			auto OutArray = TArray{
				FBuildingConnection(SelectionInput.DisplayedName, "", Integer)
			};

			for(int i = 0; OutArray.Num() < FoundIndex; i++)
			{
				OutArray.Add(FBuildingConnection(FString::FromInt(i), "", Type));
			}

			return OutArray;
		}
	}

	int FindSelectorIndex()
	{
		if(InputConnections.Num() <= 1) return INDEX_NONE;

		for(int i = InputConnections.Num() - 1; i > 0; i--)
		{
			if(InputConnections[i].IsValid()) return i;
		}

		return INDEX_NONE;
	}
	
	UPROPERTY(Replicated, SaveGame)
	UCCDynamicValueBase* Out;
};
