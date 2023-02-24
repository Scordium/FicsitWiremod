// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RisingEdgeDetector.generated.h"

UCLASS()
class FICSITWIREMOD_API ARisingEdgeDetector : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		bool Value = WM::GetFunctionBoolResult(GetConnection(0));

		if(Value)
		{
			if(HasChangedLastFrame) Out = false;
			else
			{
				Out = true;
				HasChangedLastFrame = true;
			}
		}
		else
		{
			Out = false;
			HasChangedLastFrame = false;
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ARisingEdgeDetector, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;

	UPROPERTY(VisibleInstanceOnly)
	bool HasChangedLastFrame;
};
