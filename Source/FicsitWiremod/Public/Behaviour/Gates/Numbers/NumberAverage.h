// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "NumberAverage.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberAverage : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		//0 - Average of all connected inputs
		if(CurrentStateIndex == 0)
		{
			TArray<FNewConnectionData> Connected;
			GetAllConnected(Connected);

			float Result = 0;
			for(auto Data : Connected)
			{
				Result += WM::GetFunctionNumberResult(Data);
			}

			Out = Result / Connected.Num();	
		}

		//1 - Average of all array elements
		else if(CurrentStateIndex == 1)
		{
			auto Array = WM::GetNumberArray(GetConnection(0));

			float Sum = 0;
			for (auto Element : Array) Sum += Element;
			
			Out = Sum / Array.Num();
		}
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberAverage, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
