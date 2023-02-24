// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ArrayLength.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayLength : public AFGWiremodBuildable
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = GetArrayLength(GetConnection(0));
	}

	static int GetArrayLength(const FNewConnectionData& Data)
	{
		switch (Data.ConnectionType)
		{
		case Unknown: return 0;
		case ArrayOfBoolean: return WM::GetBoolArray(Data).Num();
		case ArrayOfNumber: return WM::GetNumberArray(Data).Num();
		case ArrayOfString: return WM::GetStringArray(Data).Num();
		case ArrayOfVector: return WM::GetVectorArray(Data).Num();
		case ArrayOfColor: return WM::GetColorArray(Data).Num();
		case ArrayOfEntity: return WM::GetEntityArray(Data).Num();
		case ArrayOfInventory: return WM::GetInventoryArray(Data).Num();
		case ArrayOfStack: return WM::GetItemStackArray(Data).Num();
		case ArrayOfPowerGrid: return WM::GetPowerGridArray(Data).Num();
		case ArrayOfRecipe: return WM::GetRecipeArray(Data).Num();
		case ArrayOfItemAmount: return WM::GetItemAmountArray(Data).Num();
		default:
			UE_LOG(LogTemp, Error,TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function ARRAY_LENGTH. This should not be happening. Possibly trying to use the function outside array-focused gates?"), (int)Data.ConnectionType);
			return 0;
		}
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayLength, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	int Out;
};
