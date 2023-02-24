// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ArrayInsert.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayInsert : public AFGWiremodBuildable, public IIConstantsDistributor
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		WM::FillDynamicStructFromData(GetConnection(0), Out);
		const int Index = WM_GetInt(2);
		
		
		switch (Out.ConnectionType)
		{
		case Unknown: break;
		case ArrayOfBoolean:
			if(Out.BoolArr.IsValidIndex(Index))
				Out.BoolArr.Insert(WM_GetBool(1), Index); break;
		case ArrayOfNumber:
			if(Out.NumberArr.IsValidIndex(Index))
				Out.NumberArr.Insert(WM_GetFloat(1), Index); break;
		case ArrayOfString:
			if(Out.StringArr.IsValidIndex(Index))
				Out.StringArr.Insert(WM_GetString(1), Index); break;
		case ArrayOfColor:
			if(Out.ColorArr.IsValidIndex(Index))
				Out.ColorArr.Insert(WM_GetColor(1), Index); break;
		case ArrayOfEntity:
			if(Out.EntityArr.IsValidIndex(Index))
				Out.EntityArr.Insert(WM_GetEntity(1), Index); break;
		case ArrayOfVector:
			if(Out.VectorArr.IsValidIndex(Index))
				Out.VectorArr.Insert(WM_GetVector(1), Index); break;
		case ArrayOfStack:
			if(Out.StackArr.IsValidIndex(Index))
				Out.StackArr.Insert(WM_GetStack(1), Index); break;
		case ArrayOfInventory:
			if(Out.InventoryArr.IsValidIndex(Index))
				Out.InventoryArr.Insert(WM_GetInventory(1), Index); break;
		case ArrayOfPowerGrid:
			if(Out.PowerGridArr.IsValidIndex(Index))
				Out.PowerGridArr.Insert(WM_GetPowerCircuit(1), Index); break;
		case ArrayOfRecipe:
			if(Out.RecipeArr.IsValidIndex(Index))
				Out.RecipeArr.Insert(WM_GetRecipe(1), Index); break;
		case ArrayOfItemAmount:
			if(Out.ItemAmountArr.IsValidIndex(Index))
				Out.ItemAmountArr.Insert(WM_GetItemAmount(1), Index); break;
			
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function INSERT_ARRAY_ELEMENT. This should not be happening. Possibly trying to use the function Outside array-focused gates?"), (int)Out.ConnectionType);
			break;
		}
		
		SetOutputType(0, Out.ConnectionType);
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayInsert, Out);
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
