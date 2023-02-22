// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "TFlipFlop.generated.h"

UCLASS()
class FICSITWIREMOD_API ATFlipFlop : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		bool Clock = WM::GetFunctionBoolResult(GetConnection(0));

		if(Clock)
		{
			if(!HasClockedLastFrame)
			{
				Out = !Out;
			}
			HasClockedLastFrame = true;
		}
		else HasClockedLastFrame = false;
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ATFlipFlop, Out);
	};
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;

	UPROPERTY(VisibleInstanceOnly)
	bool HasClockedLastFrame;
};
