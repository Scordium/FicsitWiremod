// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "OR.generated.h"

UCLASS(DisplayName="Wiremod Gates | Logic | OR")
class FICSITWIREMOD_API AOR : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AOR, Out)
	};

	virtual void Process_Implementation(float DeltaTime) override
	{
		TArray<FNewConnectionData> Connected;
		GetAllConnected(Connected);
		
		for (auto Data : Connected)
		{
			if(WM::GetFunctionBoolResult(Data))
			{
				Out = true;
				return;
			}
		}
		Out = false;
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;
	
};
