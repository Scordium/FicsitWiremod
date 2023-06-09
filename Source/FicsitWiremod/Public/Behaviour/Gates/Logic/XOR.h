// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "XOR.generated.h"

UCLASS()
class FICSITWIREMOD_API AXOR : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AXOR, Out)
	};

	virtual void Process_Implementation(float DeltaTime) override
	{
		//Default state - N+ inputs, 1 output
		if(CurrentStateIndex == 0)
		{
			TArray<FConnectionData> Connected;
			GetAllConnected(Connected);

			int Active = 0;
			for (auto Data : Connected) Active += Data.GetBool();
			
			Out = (Active%2) == 1;	
		}
		//Alternate state - 1 bool array input, 1 output
		else if(CurrentStateIndex == 1)
		{
			auto Array = GetConnection(0).GetBoolArray();
			
			int Active = 0;
			for(auto Element : Array) Active += Element;
			
			Out = (Active%2) == 1;
		}
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;
};
