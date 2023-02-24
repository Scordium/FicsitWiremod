// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ArrayMake.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayMake : public AFGWiremodBuildable, public IIConstantsDistributor
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		MakeInputList();
	}
	
	virtual void Process_Implementation(float DeltaTime) override
	{
		TArray<FNewConnectionData> Elements;
		GetAllConnected(Elements);
		
		if(Elements.Num() == 0) return;
		
		Out = FDynamicValue();
		Out.ConnectionType = UWiremodUtils::BaseToArray(Elements[0].ConnectionType);

		switch (Out.ConnectionType)
		{
		case ArrayOfBoolean:
			for (FNewConnectionData element : Elements)
				Out.BoolArr.Add(WM::GetFunctionBoolResult(element));
			break;

		case ArrayOfNumber:
			for (FNewConnectionData element : Elements)
				Out.NumberArr.Add(WM::GetFunctionNumberResult(element));
			break;

		case ArrayOfString:
			for (FNewConnectionData element : Elements)
				Out.StringArr.Add(WM::GetFunctionStringResult(element));
			break;

		case ArrayOfVector:
			for (FNewConnectionData element : Elements)
				Out.VectorArr.Add(WM::GetFunctionVectorResult(element));
			break;

		case ArrayOfColor:
			for (FNewConnectionData element : Elements)
				Out.ColorArr.Add(WM::GetFunctionColorResult(element));
			break;

		case ArrayOfEntity:
			for (FNewConnectionData element : Elements)
				Out.EntityArr.Add(WM::GetFunctionEntityResult(element));
			break;

		case ArrayOfStack:
			for (FNewConnectionData element : Elements)
				Out.StackArr.Add(WM::GetFunctionStackResult(element));
			break;

		case ArrayOfInventory:
			for (FNewConnectionData element : Elements)
				Out.InventoryArr.Add(WM::GetFunctionInventory(element));
			break;

		case ArrayOfRecipe:
			for (FNewConnectionData element : Elements)
				Out.RecipeArr.Add(WM::GetFunctionRecipeResult(element));
			break;
		
		case ArrayOfPowerGrid:
			for (FNewConnectionData element : Elements)
				Out.PowerGridArr.Add(WM::GetFunctionPowerCircuitResult(element));
			break;

		case ArrayOfItemAmount:
			for (FNewConnectionData element : Elements)
				Out.ItemAmountArr.Add(WM::GetItemAmount(element));
			break;
			
		default: break;
		}
	}


	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayMake, Out);
	}

	virtual FDynamicValue GetValue_Implementation(const FString& ValueName) override{ return Out; }
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	FDynamicValue Out;

	UPROPERTY(EditDefaultsOnly)
	FText ItemDisplayNameFormat = FText::FromString("Item");

	virtual void OnInputConnected_Implementation(const FNewConnectionData& Data, int Index, UObject* Setter) override
	{
		Super::OnInputConnected_Implementation(Data, Index, Setter);
		MakeInputList();
	}

	virtual void OnInputDisconnected_Implementation(int Index, UObject* Setter) override
	{
		Super::OnInputDisconnected_Implementation(Index, Setter);
		MakeInputList();
	}
	
	void MakeInputList()
	{
		for(int i = InputConnections.Num() - 1; i >= 0; i--)
		{
			if(InputConnections[i].IsValid())
			{
				GenerateInputList(InputConnections[i].ConnectionType, i + 1);
				return;
			}
		}

		GenerateInputList(AnyNonArray);
	}


	void GenerateInputList(EConnectionType OfType, int InputCount = 8)
	{
		ConnectionsInfo.Inputs.Empty();
		if(InputCount < 8) InputCount = 8;

		for(int i = 0; i <= InputCount; i++)
		{
			auto item = FBuildingConnection(
				ItemDisplayNameFormat.ToString() + " " + FString::FromInt(i),
				"",
				OfType);
			ConnectionsInfo.Inputs.Add(item);
		}

		if(OfType == AnyNonArray) SetOutputType(0, Unknown);
		else SetOutputType(0, UWiremodUtils::BaseToArray(OfType));
	}
};
