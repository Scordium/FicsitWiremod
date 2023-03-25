// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "XNOR.generated.h"

UCLASS()
class FICSITWIREMOD_API AXNOR : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AXNOR, Out)
	};

	virtual void Process_Implementation(float DeltaTime) override
	{
		//Default state - N+ inputs, 1 output
		if(CurrentStateIndex == 0)
		{
			TArray<FNewConnectionData> Connected;
			GetAllConnected(Connected);

			int Active = 0;
			for (auto Data : Connected) Active += WM::GetFunctionBoolResult(Data);
			
			Out = (Active%2) == 0;	
		}
		//Alternate state - 1 bool array input, 1 output
		else if(CurrentStateIndex == 1)
		{
			auto Array = WM::GetBoolArray(GetConnection(0));
			
			int Active = 0;
			for(auto Element : Array) Active += Element;
			
			Out = (Active%2) == 0;
		}
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;
};
