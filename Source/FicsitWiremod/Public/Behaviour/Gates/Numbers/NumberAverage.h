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
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		//0 - Average of all connected inputs
		if(CurrentStateIndex == 0)
		{
			TArray<FConnectionData> Connected;
			GetAllConnected(Connected);

			double Result = 0;
			for(auto& Data : Connected)
			{
				Result += Data.GetFloat();
			}

			Out = Result / Connected.Num();	
		}

		//1 - Average of all array elements
		else if(CurrentStateIndex == 1)
		{
			auto Array = GetConnection(0).GetFloatArray();

			double Sum = 0;
			for (auto Element : Array) Sum += Element;
			
			Out = Sum / Array.Num();
		}
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberAverage, Out);
	}


	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
