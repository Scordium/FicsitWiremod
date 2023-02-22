// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "AddNumbers.generated.h"

UCLASS()
class FICSITWIREMOD_API AAddNumbers : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
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

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AAddNumbers, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
