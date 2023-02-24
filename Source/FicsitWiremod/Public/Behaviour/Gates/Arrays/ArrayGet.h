// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ArrayGet.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayGet : public AFGWiremodBuildable, public IIConstantsDistributor
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = GetArrayElement(GetConnection(0), WM_GetInt(1));
		SetOutputType(0, Out.ConnectionType);
	}

	//Moved implementation out of process function to allow other gates to use this.
	static FDynamicValue GetArrayElement(const FNewConnectionData& Array, const int Index)
	{
		auto Out = FDynamicValue();
		Out.ConnectionType = UWiremodUtils::ArrayToBase(Array.ConnectionType);

		
		switch (Array.ConnectionType)
		{
		case Unknown: break;
		case ArrayOfBoolean:
			{
				auto arr = WM::GetBoolArray(Array);
				if(arr.IsValidIndex(Index))
					Out.StoredBool = arr[Index];
				break;
			}
		
		case ArrayOfNumber:
			{
				auto arr = WM::GetNumberArray(Array);
				if(arr.IsValidIndex(Index))
					Out.StoredFloat = arr[Index];
				break;
			}

		case ArrayOfString:
			{
				auto arr = WM::GetStringArray(Array);
				if(arr.IsValidIndex(Index))
					Out.StoredString = arr[Index];
				break;
			}

		case ArrayOfVector:
			{
				auto arr = WM::GetVectorArray(Array);
				if(arr.IsValidIndex(Index))
					Out.StoredVector = arr[Index];
				break;
			}

		case ArrayOfColor:
			{
				auto arr = WM::GetColorArray(Array);
				if(arr.IsValidIndex(Index))
					Out.StoredColor = arr[Index];
				break;
			}

		case ArrayOfEntity:
			{
				auto arr = WM::GetEntityArray(Array);
				if(arr.IsValidIndex(Index))
					Out.Entity = arr[Index];
				break;
			}

		case ArrayOfInventory:
			{
				auto arr = WM::GetInventoryArray(Array);
				if(arr.IsValidIndex(Index))
					Out.Inventory = arr[Index];
				break;
			}

		case ArrayOfStack:
			{
				auto arr = WM::GetItemStackArray(Array);
				if(arr.IsValidIndex(Index))
					Out.Stack = arr[Index];
				break;
			}
		
		case ArrayOfPowerGrid:
			{
				auto arr = WM::GetPowerGridArray(Array);
				if(arr.IsValidIndex(Index))
					Out.PowerGrid = arr[Index];
				break;
			}

		case ArrayOfRecipe:
			{
				auto arr = WM::GetRecipeArray(Array);
				if(arr.IsValidIndex(Index))
					Out.Recipe = arr[Index];
				break;
			}
			
		case ArrayOfItemAmount:
			{
				auto arr = WM::GetItemAmountArray(Array);
				if(arr.IsValidIndex(Index))
					Out.ItemAmount = arr[Index];
				break;
			}
		
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function GET_ARRAY_ELEMENT"), (int)Out.ConnectionType);
			break;
		}

		return Out;
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayGet, Out);
	}

	virtual FDynamicValue GetValue_Implementation(const FString& ValueName) override{ return Out; }
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	FDynamicValue Out;
};
