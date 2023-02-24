// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "AND.generated.h"

UCLASS(DisplayName="Wiremod Gates | Logic | AND")
class FICSITWIREMOD_API AAND : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(AAND, Out);
	}

	virtual void Process_Implementation(float DeltaTime) override
	{
		TArray<FNewConnectionData> Connected;
		GetAllConnected(Connected);
		
		for (auto Data : Connected)
		{
			if(!WM::GetFunctionBoolResult(Data))
			{
				Out = false;
				return;
			}
		}
		Out = true;
	}

	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;
	
};
