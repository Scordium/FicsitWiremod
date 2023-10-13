// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "NOR.generated.h"

UCLASS()
class FICSITWIREMOD_API ANOR : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANOR, Out)
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
					Out = false;
					return;
				}
			}
			Out = true;	
		}
		//Alternate state - 1 bool array input, 1 output
		else if(CurrentStateIndex == 1)
		{
			auto Array = GetConnection(0).GetBoolArray();

			for (auto Element : Array)
			{
				if(Element)
				{
					Out = false;
					return;
				}
			}
			Out = true;
		}
	}
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	bool Out;
};
