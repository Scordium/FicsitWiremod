// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ArraySet.generated.h"

UCLASS()
class FICSITWIREMOD_API AArraySet : public AFGWiremodBuildable, public IIConstantsDistributor
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
				Out.BoolArr[Index] = WM_GetBool(1); break;
			
		case ArrayOfNumber:
			if(Out.NumberArr.IsValidIndex(Index))
				Out.NumberArr[Index] = WM_GetFloat(1); break;
			
		case ArrayOfString:
			if(Out.StringArr.IsValidIndex(Index))
				Out.StringArr[Index] = WM_GetString(1); break;
			
		case ArrayOfColor:
			if(Out.ColorArr.IsValidIndex(Index))
				Out.ColorArr[Index] = WM_GetColor(1); break;
			
		case ArrayOfEntity:
			if(Out.EntityArr.IsValidIndex(Index))
				Out.EntityArr[Index] = WM_GetEntity(1); break;
			
		case ArrayOfVector:
			if(Out.VectorArr.IsValidIndex(Index))
				Out.VectorArr[Index] = WM_GetVector(1); break;
			
		case ArrayOfStack:
			if(Out.StackArr.IsValidIndex(Index))
				Out.StackArr[Index] = WM_GetStack(1); break;
			
		case ArrayOfInventory:
			if(Out.InventoryArr.IsValidIndex(Index))
				Out.InventoryArr[Index] = WM_GetInventory(1); break;
			
		case ArrayOfPowerGrid:
			if(Out.PowerGridArr.IsValidIndex(Index))
				Out.PowerGridArr[Index] = WM_GetPowerCircuit(1); break;

		case ArrayOfRecipe:
			if(Out.RecipeArr.IsValidIndex(Index))
				Out.RecipeArr[Index] = WM_GetRecipe(1); break;

		case ArrayOfItemAmount:
			if(Out.ItemAmountArr.IsValidIndex(Index))
				Out.ItemAmountArr[Index] = WM_GetItemAmount(1); break;
			
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function SET_ARRAY_ELEMENT. This should not be happening. Possibly trying to use the function Outside array-focused gates?"), (int)Out.ConnectionType);
			break;
		}

		SetOutputType(0, Out.ConnectionType);
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArraySet, Out);
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
