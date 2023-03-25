// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "SinusoidalInterpolation.generated.h"

UCLASS()
class FICSITWIREMOD_API ASinusoidalInterpolation : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		float A = WM_GetFloat(0);
		float B = WM_GetFloat(1);
		float Alpha = WM_GetFloat(2);
		
		//0 - Sin IN
		if(CurrentStateIndex == 0) Out = FMath::InterpSinIn(A, B, Alpha);
		//1 - Sin IN OUT
		else if(CurrentStateIndex == 1) Out = FMath::InterpSinInOut(A, B, Alpha);
		//2 - Sin OUT
		else if(CurrentStateIndex == 2) Out = FMath::InterpSinOut(A, B, Alpha);
	}
	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ASinusoidalInterpolation, Out);
	}

	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
