// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "DivideNumbers.generated.h"

UCLASS()
class FICSITWIREMOD_API ADivideNumbers : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		//0 - Divide all connected
		if(CurrentStateIndex == 0)
		{
			TArray<FConnectionData> Connected;
			GetAllConnected(Connected);

			double Result = 0;
			for(int i = 0; i < Connected.Num(); i++)
			{
				double Value = Connected[i].GetFloat();
				Result = (i == 0) ? Value : Result / Value;
			}

			Out = Result;	
		}

		//1 - Divide all array elements
		else if(CurrentStateIndex == 1)
		{
			auto Array = GetConnection(0).GetFloatArray();

			double Result = 0;
			for(int i = 0; i < Array.Num(); i++)
			{
				Result = (i == 0) ? Array[i] : Result / Array[i];
			}
		}

		//2 - Divide each array element by a value
		else if(CurrentStateIndex == 2)
		{
			auto Array = GetConnection(0).GetFloatArray();
			double Value = GetConnection(1).GetFloat();;
			for(int i = 0; i < Array.Num(); i++) Array[i] /= Value;

			Out_Array = Array;
		}
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ADivideNumbers, Out);
		DOREPLIFETIME(ADivideNumbers, Out_Array);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;

	UPROPERTY(Replicated, VisibleInstanceOnly)
	TArray<double> Out_Array;
};
