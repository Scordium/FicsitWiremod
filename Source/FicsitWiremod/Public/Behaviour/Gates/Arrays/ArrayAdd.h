// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ArrayAdd.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayAdd : public AFGWiremodBuildable, public IIConstantsDistributor
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		WM::FillDynamicStructFromData(GetConnection(0), Out);

		switch (Out.ConnectionType)
		{
		case Unknown: break;
		case ArrayOfBoolean: Out.BoolArr.Add(WM_GetBool(1)); break;
		case ArrayOfNumber: Out.NumberArr.Add(WM_GetFloat(1)); break;
		case ArrayOfString: Out.StringArr.Add(WM_GetString(1)); break;
		case ArrayOfColor: Out.ColorArr.Add(WM_GetColor(1)); break;
		case ArrayOfEntity: Out.EntityArr.Add(WM_GetEntity(1)); break;
		case ArrayOfVector: Out.VectorArr.Add(WM_GetVector(1)); break;
		case ArrayOfStack: Out.StackArr.Add(WM_GetStack(1)); break;
		case ArrayOfInventory: Out.InventoryArr.Add(WM_GetInventory(1)); break;
		case ArrayOfPowerGrid: Out.PowerGridArr.Add(WM_GetPowerCircuit(1)); break;
		case ArrayOfRecipe: Out.RecipeArr.Add(WM_GetRecipe(1)); break;
		case ArrayOfItemAmount: Out.ItemAmountArr.Add(WM_GetItemAmount(1)); break;
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function ADD_ARRAY_ELEMENT. This should not be happening. Possibly trying to use the function outside array-focused gates?"), (int)Out.ConnectionType);
			break;
		}

		SetOutputType(0, Out.ConnectionType);
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayAdd, Out);
	}

	virtual FDynamicValue GetValue_Implementation(const FString& ValueName) override{ return Out; }

	virtual void OnInputConnected_Implementation(const FNewConnectionData& Data, int Index, UObject* Setter) override
	{
		//If a new array was connected
		if(Index == 0)
		{
			//Disconnect the previous element input if the array type is not compatible with it.
			if(!UWiremodUtils::IsValidConnectionPair(GetConnection(0).ConnectionType, Data.ConnectionType))
				OnInputDisconnected_Internal(1);
		}
		 Super::OnInputConnected_Implementation(Data, Index, Setter);
		SetInputType(1, UWiremodUtils::ArrayToBase(Data.ConnectionType));
	}
    	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	FDynamicValue Out;
};
