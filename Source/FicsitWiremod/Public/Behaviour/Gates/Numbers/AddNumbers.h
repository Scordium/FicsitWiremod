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
	virtual void Process_Implementation(float DeltaTime) override
	{

		//0 - Sum of all connected inputs
		if(CurrentStateIndex == 0)
		{
			TArray<FNewConnectionData> Connected;
			GetAllConnected(Connected);

			float Output = 0;
			for (auto Data : Connected)
			{
				Output += WM::GetFunctionNumberResult(Data);
			}

			Out = Output;
		}

		//1 - Array sum
		else if(CurrentStateIndex == 1)
		{
			auto Array = WM::GetNumberArray(GetConnection(0));
			float Output = 0;

			for(auto Element : Array) Output += Element;
			Out = Output;
		}

		//2 - Add a value to all elements in array
		else if(CurrentStateIndex == 2)
		{
			auto Array = WM::GetNumberArray(GetConnection(0));
			float Value = WM_GetFloat(1);

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
	float Out;

	UPROPERTY(Replicated, VisibleInstanceOnly)
	TArray<float> Out_Array;
};
