// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "AND.generated.h"

UCLASS(DisplayName="Wiremod Gates | Logic | AND")
class FICSITWIREMOD_API AAND : public AMultistateWiremodBuildable
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
		//Default state - N+ inputs, 1 output
		if(CurrentStateIndex == 0)
		{
			TArray<FConnectionData> Connected;
			GetAllConnected(Connected);
		
			for (auto Data : Connected)
			{
				if(!Data.GetBool())
				{
					Out = false;
					return;
				}
			}
			Out = true;
		}
		//Alternate state - 1 bool array input, 1 output
		else if (CurrentStateIndex == 1)
		{
			auto Array = GetConnection(0).GetBoolArray();

			for (auto Element : Array)
			{
				if(!Element)
				{
					Out = false;
					return;
				}
			}
			Out = true;
		}
	}

	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;
	
};
