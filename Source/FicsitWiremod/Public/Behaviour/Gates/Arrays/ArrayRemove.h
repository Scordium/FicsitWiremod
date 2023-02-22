// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ArrayRemove.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayRemove : public AFGWiremodBuildable, public IIConstantsDistributor
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		WM::FillDynamicStructFromData(GetConnection(0), Out);

		const int Index = WM_GetInt(1);
		
		switch (Out.ConnectionType)
		{
		case Unknown: break;
		case ArrayOfBoolean:
			if(Out.BoolArr.IsValidIndex(Index))
				Out.BoolArr.RemoveAt(Index); break;
		case ArrayOfNumber:
			if(Out.NumberArr.IsValidIndex(Index))
				Out.NumberArr.RemoveAt(Index); break;
		case ArrayOfString:
			if(Out.StringArr.IsValidIndex(Index))
				Out.StringArr.RemoveAt(Index); break;
		case ArrayOfColor:
			if(Out.ColorArr.IsValidIndex(Index))
				Out.ColorArr.RemoveAt(Index); break;
		case ArrayOfEntity:
			if(Out.EntityArr.IsValidIndex(Index))
				Out.EntityArr.RemoveAt(Index); break;
		case ArrayOfVector:
			if(Out.VectorArr.IsValidIndex(Index))
				Out.VectorArr.RemoveAt(Index); break;
		case ArrayOfStack:
			if(Out.StackArr.IsValidIndex(Index))
				Out.StackArr.RemoveAt(Index); break;
		case ArrayOfInventory:
			if(Out.InventoryArr.IsValidIndex(Index))
				Out.InventoryArr.RemoveAt(Index); break;
		case ArrayOfPowerGrid:
			if(Out.PowerGridArr.IsValidIndex(Index))
				Out.PowerGridArr.RemoveAt(Index); break;
		case ArrayOfRecipe:
			if(Out.RecipeArr.IsValidIndex(Index))
				Out.RecipeArr.RemoveAt(Index); break;
		case ArrayOfItemAmount:
			if(Out.ItemAmountArr.IsValidIndex(Index))
				Out.ItemAmountArr.RemoveAt(Index); break;
			
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function REMOVE_ARRAY_ELEMENT. This should not be happening. Possibly trying to use the function outside array-focused gates?"), (int)Out.ConnectionType);
			break;
		}
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayRemove, Out);
	}

	virtual FDynamicValue GetValue_Implementation(const FString& ValueName) override{ return Out; }
    	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	FDynamicValue Out;
};
