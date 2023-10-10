// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "CommonLib/InterpolationFuncs.h"
#include "BounceInterpolation.generated.h"

UCLASS()
class FICSITWIREMOD_API ABounceInterpolation : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto A = GetConnection(0).GetFloat();
		auto B = GetConnection(1).GetFloat();
		auto Alpha = GetConnection(2).GetFloat();
		
		//Bounce IN
		if(CurrentStateIndex == 0) Out = UInterpolationFuncs::BounceIn(A, B, Alpha);
		//Bounce IN OUT
		else if(CurrentStateIndex == 1) Out = UInterpolationFuncs::BounceInOut(A, B, Alpha);
		//Bounce OUT
		else if(CurrentStateIndex == 2) Out = UInterpolationFuncs::BounceOut(A, B, Alpha);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABounceInterpolation, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	double Out;
};
