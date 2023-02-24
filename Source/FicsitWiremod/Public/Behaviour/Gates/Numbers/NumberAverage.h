// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberAverage.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberAverage : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
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

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberAverage, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
