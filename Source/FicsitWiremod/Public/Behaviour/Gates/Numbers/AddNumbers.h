// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "AddNumbers.generated.h"

UCLASS()
class FICSITWIREMOD_API AAddNumbers : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{

		//0 - Sum of all connected inputs
		if(CurrentStateIndex == 0)
		{
			TArray<FConnectionData> Connected;
			GetAllConnected(Connected);

			double Output = 0;
			for (auto Data : Connected)
			{
				Output += Data.GetFloat();
			}

			Out = Output;
		}

		//1 - Array sum
		else if(CurrentStateIndex == 1)
		{
			auto Array = GetConnection(0).GetFloatArray();
			double Output = 0;

			for(auto Element : Array) Output += Element;
			Out = Output;
		}

		//2 - Add a value to all elements in array
		else if(CurrentStateIndex == 2)
		{
			auto Array = GetConnection(0).GetFloatArray();
			double Value = GetConnection(1).GetFloat();;

			for(int i = 0; i < Array.Num(); i++) Array[i] += Value;

			Out_Array = Array;
		}
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AAddNumbers, Out);
		DOREPLIFETIME(AAddNumbers, Out_Array);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;

	UPROPERTY(Replicated, VisibleInstanceOnly)
	TArray<double> Out_Array;
};
