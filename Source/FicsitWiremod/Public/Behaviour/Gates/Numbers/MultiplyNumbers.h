// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "MultiplyNumbers.generated.h"

UCLASS()
class FICSITWIREMOD_API AMultiplyNumbers : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		TArray<FNewConnectionData> Connected;
		GetAllConnected(Connected);

		float Result = 0;
		for(int i = 0; i < Connected.Num(); i++)
		{
			Result = (i == 0) ? WM_GetFloat(i) : Result * WM_GetFloat(i);
		}

		Out = Result;
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AMultiplyNumbers, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
