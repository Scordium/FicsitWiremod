// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "MultiplyNumbers.generated.h"

UCLASS()
class FICSITWIREMOD_API AMultiplyNumbers : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{

		//0 - Multiply all connected
		if(CurrentStateIndex == 0)
		{
			TArray<FConnectionData> Connected;
			GetAllConnected(Connected);

			double Result = 0;
			for(int i = 0; i < Connected.Num(); i++)
			{
				double Value = Connected[i].GetFloat();
				Result = (i == 0) ? Value : Result * Value;
			}

			Out = Result;	
		}

		//1 - Multiply all array elements
		else if(CurrentStateIndex == 1)
		{
			auto Array = GetConnection(0).GetFloatArray();

			double Result = 0;
			for(int i = 0; i < Array.Num(); i++)
			{
				Result = (i == 0) ? Array[i] : Result * Array[i];
			}
		}

		//2 - Multiply each array element by a value
		else if(CurrentStateIndex == 2)
		{
			auto Array = GetConnection(0).GetFloatArray();
			double Value = GetConnection(1).GetFloat();;
			for(int i = 0; i < Array.Num(); i++) Array[i] *= Value;

			Out_Array = Array;
		}
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AMultiplyNumbers, Out);
		DOREPLIFETIME(AMultiplyNumbers, Out_Array);
	}


	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;

	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	TArray<double> Out_Array;
};
