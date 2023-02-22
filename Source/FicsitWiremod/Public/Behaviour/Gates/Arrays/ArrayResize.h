// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ArrayResize.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayResize : public AFGWiremodBuildable, public IIConstantsDistributor
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		WM::FillDynamicStructFromData(GetConnection(0), Out);
		const int NewSize = WM_GetInt(1);

		switch (Out.ConnectionType)
		{
		case Unknown: break;
		case ArrayOfBoolean: Out.BoolArr.SetNum(NewSize); break;
		case ArrayOfNumber: Out.NumberArr.SetNum(NewSize); break;
		case ArrayOfString: Out.StringArr.SetNum(NewSize); break;
		case ArrayOfVector: Out.VectorArr.SetNum(NewSize); break;
		case ArrayOfColor: Out.ColorArr.SetNum(NewSize); break;
		case ArrayOfEntity: Out.EntityArr.SetNum(NewSize); break;
		case ArrayOfInventory: Out.InventoryArr.SetNum(NewSize); break;
		case ArrayOfStack: Out.StackArr.SetNum(NewSize); break;
		case ArrayOfPowerGrid: Out.PowerGridArr.SetNum(NewSize); break;
		case ArrayOfRecipe: Out.RecipeArr.SetNum(NewSize); break;
		case ArrayOfItemAmount: Out.ItemAmountArr.SetNum(NewSize); break;
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function RESIZE_ARRAY. This should not be happening. Possibly trying to use the function Outside array-focused gates?"), (int)Out.ConnectionType);
			break;
		}
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayResize, Out);
	}

	virtual FDynamicValue GetValue_Implementation(const FString& ValueName) override{ return Out; }
    	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	FDynamicValue Out;
};
