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
	virtual void Process_Implementation(float DeltaTime) override
	{

		//0 - Multiply all connected
		if(CurrentStateIndex == 0)
		{
			TArray<FNewConnectionData> Connected;
			GetAllConnected(Connected);

			float Result = 0;
			for(int i = 0; i < Connected.Num(); i++)
			{
				float Value = WM::GetFunctionNumberResult(Connected[i]);
				Result = (i == 0) ? Value : Result * Value;
			}

			Out = Result;	
		}

		//1 - Multiply all array elements
		else if(CurrentStateIndex == 1)
		{
			auto Array = WM::GetNumberArray(GetConnection(0));

			float Result = 0;
			for(int i = 0; i < Array.Num(); i++)
			{
				Result = (i == 0) ? Array[i] : Result * Array[i];
			}
		}

		//2 - Multiply each array element by a value
		else if(CurrentStateIndex == 2)
		{
			auto Array = WM::GetNumberArray(GetConnection(0));
			float Value = WM_GetFloat(1);
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


	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;

	UPROPERTY(Replicated, VisibleInstanceOnly)
	TArray<float> Out_Array;
};
