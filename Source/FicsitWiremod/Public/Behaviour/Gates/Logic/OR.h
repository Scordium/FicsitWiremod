// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "OR.generated.h"

UCLASS(DisplayName="Wiremod Gates | Logic | OR")
class FICSITWIREMOD_API AOR : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AOR, Out)
	};

	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		//Default state - N+ inputs, 1 output
		if(CurrentStateIndex == 0)
		{
			TArray<FConnectionData> Connected;
			GetAllConnected(Connected);
		
			for (auto Data : Connected)
			{
				if(Data.GetBool())
				{
					Out = true;
					return;
				}
			}
			Out = false;	
		}
		//Alternate state - 1 bool array input, 1 output
		else if(CurrentStateIndex == 1)
		{
			auto Array = GetConnection(0).GetBoolArray();

			for (auto Element : Array)
			{
				if(Element)
				{
					Out = true;
					return;
				}
			}
			Out = false;
		}
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;
	
};
